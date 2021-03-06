// Copyright 2021 The Pigweed Authors
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>

#include "gtest/gtest.h"
#include "pw_assert/check.h"
#include "pw_bytes/array.h"
#include "pw_log/log.h"
#include "pw_rpc/integration_testing.h"
#include "pw_status/status.h"
#include "pw_sync/binary_semaphore.h"
#include "pw_thread_stl/options.h"
#include "pw_transfer/client.h"
#include "pw_transfer_test/test_server.raw_rpc.pb.h"

namespace pw::transfer {
namespace {

using namespace std::chrono_literals;

// TODO(hepler): Use more iterations when the pw_transfer synchronization issues
//     that make this flaky are fixed.
constexpr int kIterations = 1;

constexpr auto kData512 = bytes::Initialized<512>([](size_t i) { return i; });
constexpr auto kData8192 = bytes::Initialized<8192>([](size_t i) { return i; });

std::filesystem::path directory;

// Reads the file that represents the transfer with the specific ID.
std::string GetContent(uint32_t transfer_id) {
  std::ifstream stream(directory / std::to_string(transfer_id),
                       std::ios::binary | std::ios::ate);
  std::string contents(stream.tellg(), '\0');

  stream.seekg(0, std::ios::beg);
  PW_CHECK(stream.read(contents.data(), contents.size()));

  return contents;
}

// Drops the null terminator from a string literal.
template <size_t kLengthWithNull>
ConstByteSpan AsByteSpan(const char (&data)[kLengthWithNull]) {
  constexpr size_t kLength = kLengthWithNull - 1;
  PW_CHECK_INT_EQ('\0', data[kLength], "Expecting null for last character");
  return std::as_bytes(std::span(data, kLength));
}

constexpr ConstByteSpan AsByteSpan(ConstByteSpan data) { return data; }

// Test fixture for pw_transfer tests. Clears the transfer files before and
// after each test.
class TransferIntegration : public ::testing::Test {
 protected:
  TransferIntegration()
      : client_(rpc::integration_test::client(),
                rpc::integration_test::kChannelId,
                work_queue_,
                client_buffer_,
                256),
        test_server_client_(rpc::integration_test::client(),
                            rpc::integration_test::kChannelId),
        work_queue_thread_(kThreadOptions, work_queue_) {
    ClearFiles();
  }

  ~TransferIntegration() {
    work_queue_.RequestStop();
    work_queue_thread_.join();

    ClearFiles();
  }

  // Sets the content of a transfer ID and returns a MemoryReader for that data.
  template <typename T>
  void SetContent(uint32_t transfer_id, const T& content) {
    const ConstByteSpan data = AsByteSpan(content);
    std::ofstream stream(directory / std::to_string(transfer_id),
                         std::ios::binary);
    PW_CHECK(
        stream.write(reinterpret_cast<const char*>(data.data()), data.size()));

    sync::BinarySemaphore reload_complete;
    rpc::RawUnaryReceiver call = test_server_client_.ReloadTransferFiles(
        {}, [&reload_complete](ConstByteSpan, Status) {
          reload_complete.release();
        });
    PW_CHECK(reload_complete.try_acquire_for(3s));
  }

  auto OnCompletion() {
    return [this](Status status) {
      last_status_ = status;
      completed_.release();
    };
  }

  // Checks that a read transfer succeeded and that the data matches the
  // expected data.
  void ExpectReadData(ConstByteSpan expected) {
    ASSERT_EQ(WaitForCompletion(), OkStatus());
    ASSERT_EQ(expected.size(), read_buffer_.size());

    EXPECT_TRUE(std::equal(read_buffer_.begin(),
                           read_buffer_.end(),
                           std::as_bytes(std::span(expected)).begin()));
  }

  // Checks that a write transfer succeeded and that the written contents match.
  void ExpectWriteData(uint32_t transfer_id, ConstByteSpan expected) {
    ASSERT_EQ(WaitForCompletion(), OkStatus());

    const std::string written = GetContent(transfer_id);
    ASSERT_EQ(expected.size(), written.size());

    ConstByteSpan bytes = std::as_bytes(std::span(written));
    EXPECT_TRUE(std::equal(bytes.begin(), bytes.end(), expected.begin()));
  }

  // Waits for the transfer to complete and returns the status.
  Status WaitForCompletion() {
    PW_CHECK(completed_.try_acquire_for(3s));
    return last_status_;
  }

  // Exact match the size of kData8192 to test filling the receiving buffer.
  stream::MemoryWriterBuffer<kData8192.size()> read_buffer_;

  Client& client() { return client_; }

 private:
  static void ClearFiles() {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
      if (!entry.is_regular_file()) {
        continue;
      }

      if (const std::string name = entry.path().filename().string();
          std::all_of(name.begin(), name.end(), [](char c) {
            return std::isdigit(c);
          })) {
        PW_LOG_DEBUG("Clearing transfer file %s", name.c_str());
        std::filesystem::remove(entry.path());
      }
    }
  }

  static constexpr thread::stl::Options kThreadOptions;

  work_queue::WorkQueueWithBuffer<4> work_queue_;

  Client client_;

  pw_rpc::raw::TestServer::Client test_server_client_;
  Status last_status_ = Status::Unknown();
  sync::BinarySemaphore completed_;
  std::byte client_buffer_[512];

  thread::Thread work_queue_thread_;
};

TEST_F(TransferIntegration, Read_UnknownId) {
  SetContent(123, "hello");

  ASSERT_EQ(OkStatus(), client().Read(456, read_buffer_, OnCompletion()));

  EXPECT_EQ(Status::NotFound(), WaitForCompletion());
}

#define PW_TRANSFER_TEST_READ(name, content)                            \
  TEST_F(TransferIntegration, Read_##name) {                            \
    for (int i = 0; i < kIterations; ++i) {                             \
      const ConstByteSpan data = AsByteSpan(content);                   \
      SetContent(__LINE__, data);                                       \
      ASSERT_EQ(OkStatus(),                                             \
                client().Read(__LINE__, read_buffer_, OnCompletion())); \
      ExpectReadData(data);                                             \
      read_buffer_.clear();                                             \
    }                                                                   \
  }                                                                     \
  static_assert(true, "Semicolons are required")

PW_TRANSFER_TEST_READ(Empty, "");
PW_TRANSFER_TEST_READ(SingleByte_1, "\0");
PW_TRANSFER_TEST_READ(SingleByte_2, "?");
PW_TRANSFER_TEST_READ(SmallData, "hunter2");
PW_TRANSFER_TEST_READ(LargeData, kData512);

TEST_F(TransferIntegration, Write_UnknownId) {
  constexpr std::byte kData[] = {std::byte{0}, std::byte{1}, std::byte{2}};
  stream::MemoryReader reader(kData);

  ASSERT_EQ(OkStatus(), client().Write(99, reader, OnCompletion()));
  EXPECT_EQ(Status::NotFound(), WaitForCompletion());

  SetContent(99, "something");
  ASSERT_EQ(OkStatus(), client().Write(100, reader, OnCompletion()));
  EXPECT_EQ(Status::NotFound(), WaitForCompletion());
}

#define PW_TRANSFER_TEST_WRITE(name, content)                                  \
  TEST_F(TransferIntegration, Write_##name) {                                  \
    for (int i = 0; i < kIterations; ++i) {                                    \
      SetContent(__LINE__, "This is junk data that should be overwritten!");   \
      const ConstByteSpan data = AsByteSpan(content);                          \
      stream::MemoryReader reader(data);                                       \
      ASSERT_EQ(OkStatus(), client().Write(__LINE__, reader, OnCompletion())); \
      ExpectWriteData(__LINE__, data);                                         \
    }                                                                          \
  }                                                                            \
  static_assert(true, "Semicolons are required")

PW_TRANSFER_TEST_WRITE(Empty, "");
PW_TRANSFER_TEST_WRITE(SingleByte_1, "\0");
PW_TRANSFER_TEST_WRITE(SingleByte_2, "?");
PW_TRANSFER_TEST_WRITE(SmallData, "hunter2");
PW_TRANSFER_TEST_WRITE(LargeData, kData512);

}  // namespace
}  // namespace pw::transfer

int main(int argc, char* argv[]) {
  if (!pw::rpc::integration_test::InitializeClient(argc, argv, "PORT DIRECTORY")
           .ok()) {
    return 1;
  }

  if (argc != 3) {
    PW_LOG_INFO("Usage: %s PORT DIRECTORY", argv[0]);
    return 1;
  }

  pw::transfer::directory = argv[2];

  return RUN_ALL_TESTS();
}
