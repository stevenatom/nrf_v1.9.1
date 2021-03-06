# Copyright 2020 The Pigweed Authors
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

# Do not rely on the PW_ROOT environment variable being set through bootstrap.
# Regardless of whether it's set or not the following include will ensure it is.
include(${CMAKE_CURRENT_LIST_DIR}/../../pw_build/pigweed.cmake)

pw_set_backend(pw_assert pw_assert_log)
pw_set_backend(pw_chrono.system_clock pw_chrono_stl.system_clock)
pw_set_backend(pw_log pw_log_basic)
pw_set_backend(pw_rpc.system_server targets.host.system_rpc_server)
pw_set_backend(pw_sync.mutex pw_sync_stl.mutex_backend)
pw_set_backend(pw_sys_io pw_sys_io_stdio)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
