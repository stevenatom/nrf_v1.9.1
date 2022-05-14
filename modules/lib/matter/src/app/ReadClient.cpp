/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *      This file defines the initiator side of a CHIP Read Interaction.
 *
 */

#include "lib/core/CHIPTLVTypes.h"
#include <app/AppBuildConfig.h>
#include <app/InteractionModelEngine.h>
#include <app/ReadClient.h>
#include <app/StatusResponse.h>

namespace chip {
namespace app {

ReadClient::ReadClient(InteractionModelEngine * apImEngine, Messaging::ExchangeManager * apExchangeMgr, Callback & apCallback,
                       InteractionType aInteractionType) :
    mpCallback(apCallback)
{
    // Error if already initialized.
    mpExchangeMgr    = apExchangeMgr;
    mpCallback       = apCallback;
    mInteractionType = aInteractionType;

    mpImEngine = apImEngine;

    if (aInteractionType == InteractionType::Subscribe)
    {
        mpImEngine->AddReadClient(this);
    }
}

ReadClient::~ReadClient()
{
    Abort();

    if (IsSubscriptionType())
    {
        CancelLivenessCheckTimer();

        //
        // Only remove ourselves from the engine's tracker list if we still continue to have a valid pointer to it.
        // This won't be the case if the engine shut down before this destructor was called (in which case, mpImEngine
        // will point to null)
        //
        if (mpImEngine)
        {
            mpImEngine->RemoveReadClient(this);
        }
    }
}

void ReadClient::Close(CHIP_ERROR aError)
{
    // OnDone below can destroy us before we unwind all the way back into the
    // exchange code and it tries to close itself.  Make sure that it doesn't
    // try to notify us that it's closing, since we will be dead.
    //
    // For more details, see #10344.
    if (mpExchangeCtx != nullptr)
    {
        mpExchangeCtx->SetDelegate(nullptr);
    }

    mpExchangeCtx = nullptr;

    if (aError != CHIP_NO_ERROR)
    {
        mpCallback.OnError(this, aError);
    }

    mpCallback.OnDone(this);
}

const char * ReadClient::GetStateStr() const
{
#if CHIP_DETAIL_LOGGING
    switch (mState)
    {
    case ClientState::Idle:
        return "Idle";
    case ClientState::AwaitingInitialReport:
        return "AwaitingInitialReport";
    case ClientState::AwaitingSubscribeResponse:
        return "AwaitingSubscribeResponse";
    case ClientState::SubscriptionActive:
        return "SubscriptionActive";
    }
#endif // CHIP_DETAIL_LOGGING
    return "N/A";
}

void ReadClient::MoveToState(const ClientState aTargetState)
{
    mState = aTargetState;
    ChipLogDetail(DataManagement, "%s ReadClient[%p]: Moving to [%10.10s]", __func__, this, GetStateStr());
}

CHIP_ERROR ReadClient::SendRequest(ReadPrepareParams & aReadPrepareParams)
{
    if (mInteractionType == InteractionType::Read)
    {
        return SendReadRequest(aReadPrepareParams);
    }

    if (mInteractionType == InteractionType::Subscribe)
    {
        return SendSubscribeRequest(aReadPrepareParams);
    }

    return CHIP_ERROR_INVALID_ARGUMENT;
}

CHIP_ERROR ReadClient::SendReadRequest(ReadPrepareParams & aReadPrepareParams)
{
    // TODO: SendRequest parameter is too long, need to have the structure to represent it
    CHIP_ERROR err = CHIP_NO_ERROR;
    System::PacketBufferHandle msgBuf;
    ChipLogDetail(DataManagement, "%s ReadClient[%p]: Sending Read Request", __func__, this);

    VerifyOrReturnError(ClientState::Idle == mState, err = CHIP_ERROR_INCORRECT_STATE);

    {
        System::PacketBufferTLVWriter writer;
        ReadRequestMessage::Builder request;

        msgBuf = System::PacketBufferHandle::New(kMaxSecureSduLengthBytes);
        VerifyOrReturnError(!msgBuf.IsNull(), err = CHIP_ERROR_NO_MEMORY);

        writer.Init(std::move(msgBuf));

        ReturnErrorOnFailure(request.Init(&writer));

        if (aReadPrepareParams.mAttributePathParamsListSize != 0 && aReadPrepareParams.mpAttributePathParamsList != nullptr)
        {
            AttributePathIBs::Builder & attributePathListBuilder = request.CreateAttributeRequests();
            ReturnErrorOnFailure(err = request.GetError());
            ReturnErrorOnFailure(GenerateAttributePathList(attributePathListBuilder, aReadPrepareParams.mpAttributePathParamsList,
                                                           aReadPrepareParams.mAttributePathParamsListSize));
        }

        if (aReadPrepareParams.mEventPathParamsListSize != 0 && aReadPrepareParams.mpEventPathParamsList != nullptr)
        {
            EventPathIBs::Builder & eventPathListBuilder = request.CreateEventRequests();
            ReturnErrorOnFailure(err = request.GetError());

            ReturnErrorOnFailure(GenerateEventPaths(eventPathListBuilder, aReadPrepareParams.mpEventPathParamsList,
                                                    aReadPrepareParams.mEventPathParamsListSize));

            if (aReadPrepareParams.mEventNumber != 0)
            {
                // EventFilter is optional
                EventFilterIBs::Builder & eventFilters = request.CreateEventFilters();
                ReturnErrorOnFailure(request.GetError());

                EventFilterIB::Builder & eventFilter = eventFilters.CreateEventFilter();
                ReturnErrorOnFailure(eventFilters.GetError());
                ReturnErrorOnFailure(eventFilter.EventMin(aReadPrepareParams.mEventNumber).EndOfEventFilterIB().GetError());
                ReturnErrorOnFailure(eventFilters.EndOfEventFilters().GetError());
            }
        }

        ReturnErrorOnFailure(request.IsFabricFiltered(aReadPrepareParams.mIsFabricFiltered).EndOfReadRequestMessage().GetError());
        ReturnErrorOnFailure(writer.Finalize(&msgBuf));
    }

    mpExchangeCtx = mpExchangeMgr->NewContext(aReadPrepareParams.mSessionHolder.Get(), this);
    VerifyOrReturnError(mpExchangeCtx != nullptr, err = CHIP_ERROR_NO_MEMORY);

    mpExchangeCtx->SetResponseTimeout(aReadPrepareParams.mTimeout);

    ReturnErrorOnFailure(mpExchangeCtx->SendMessage(Protocols::InteractionModel::MsgType::ReadRequest, std::move(msgBuf),
                                                    Messaging::SendFlags(Messaging::SendMessageFlags::kExpectResponse)));

    mPeerNodeId  = aReadPrepareParams.mSessionHolder->AsSecureSession()->GetPeerNodeId();
    mFabricIndex = aReadPrepareParams.mSessionHolder->AsSecureSession()->GetFabricIndex();

    MoveToState(ClientState::AwaitingInitialReport);

    return CHIP_NO_ERROR;
}

CHIP_ERROR ReadClient::GenerateEventPaths(EventPathIBs::Builder & aEventPathsBuilder, EventPathParams * apEventPathParamsList,
                                          size_t aEventPathParamsListSize)
{
    for (size_t index = 0; index < aEventPathParamsListSize; ++index)
    {
        VerifyOrReturnError(apEventPathParamsList[index].IsValidEventPath(), CHIP_ERROR_IM_MALFORMED_ATTRIBUTE_PATH);
        EventPathIB::Builder & path = aEventPathsBuilder.CreatePath();
        ReturnErrorOnFailure(aEventPathsBuilder.GetError());
        ReturnErrorOnFailure(path.Encode(apEventPathParamsList[index]));
    }

    aEventPathsBuilder.EndOfEventPaths();
    return aEventPathsBuilder.GetError();
}

CHIP_ERROR ReadClient::GenerateAttributePathList(AttributePathIBs::Builder & aAttributePathIBsBuilder,
                                                 AttributePathParams * apAttributePathParamsList,
                                                 size_t aAttributePathParamsListSize)
{
    for (size_t index = 0; index < aAttributePathParamsListSize; index++)
    {
        VerifyOrReturnError(apAttributePathParamsList[index].IsValidAttributePath(), CHIP_ERROR_IM_MALFORMED_ATTRIBUTE_PATH);
        AttributePathIB::Builder & path = aAttributePathIBsBuilder.CreatePath();
        ReturnErrorOnFailure(aAttributePathIBsBuilder.GetError());
        ReturnErrorOnFailure(path.Encode(apAttributePathParamsList[index]));
    }

    aAttributePathIBsBuilder.EndOfAttributePathIBs();
    return aAttributePathIBsBuilder.GetError();
}

CHIP_ERROR ReadClient::OnMessageReceived(Messaging::ExchangeContext * apExchangeContext, const PayloadHeader & aPayloadHeader,
                                         System::PacketBufferHandle && aPayload)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    VerifyOrExit(!IsIdle(), err = CHIP_ERROR_INCORRECT_STATE);

    if (aPayloadHeader.HasMessageType(Protocols::InteractionModel::MsgType::ReportData))
    {
        err = ProcessReportData(std::move(aPayload));
        SuccessOrExit(err);
    }
    else if (aPayloadHeader.HasMessageType(Protocols::InteractionModel::MsgType::SubscribeResponse))
    {
        VerifyOrExit(apExchangeContext == mpExchangeCtx, err = CHIP_ERROR_INCORRECT_STATE);
        err = ProcessSubscribeResponse(std::move(aPayload));

        // Forget the context as SUBSCRIBE RESPONSE is the last message in SUBSCRIBE transaction and
        // ExchangeContext::HandleMessage automatically closes a context if no other messages need to
        // be sent or received.
        mpExchangeCtx = nullptr;
        SuccessOrExit(err);
    }
    else if (aPayloadHeader.HasMessageType(Protocols::InteractionModel::MsgType::StatusResponse))
    {
        VerifyOrExit(apExchangeContext == mpExchangeCtx, err = CHIP_ERROR_INCORRECT_STATE);
        err = StatusResponse::ProcessStatusResponse(std::move(aPayload));
        SuccessOrExit(err);
    }
    else
    {
        err = CHIP_ERROR_INVALID_MESSAGE_TYPE;
    }

exit:
    if ((!IsSubscriptionType() && !mPendingMoreChunks) || err != CHIP_NO_ERROR)
    {
        Close(err);
    }

    return err;
}

void ReadClient::Abort()
{
    //
    // If the exchange context hasn't already been gracefully closed
    // (signaled by setting it to null), then we need to forcibly
    // tear it down.
    //
    if (mpExchangeCtx != nullptr)
    {
        // We might be a delegate for this exchange, and we don't want the
        // OnExchangeClosing notification in that case.  Null out the delegate
        // to avoid that.
        //
        // TODO: This makes all sorts of assumptions about what the delegate is
        // (notice the "might" above!) that might not hold in practice.  We
        // really need a better solution here....
        mpExchangeCtx->SetDelegate(nullptr);
        mpExchangeCtx->Abort();
        mpExchangeCtx = nullptr;
    }
}

CHIP_ERROR ReadClient::OnUnsolicitedReportData(Messaging::ExchangeContext * apExchangeContext,
                                               System::PacketBufferHandle && aPayload)
{
    mpExchangeCtx = apExchangeContext;

    CHIP_ERROR err = ProcessReportData(std::move(aPayload));
    if (err != CHIP_NO_ERROR)
    {
        Close(err);
    }

    return err;
}

CHIP_ERROR ReadClient::ProcessReportData(System::PacketBufferHandle && aPayload)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    ReportDataMessage::Parser report;

    bool isEventReportsPresent       = false;
    bool isAttributeReportIBsPresent = false;
    bool suppressResponse            = true;
    uint64_t subscriptionId          = 0;
    EventReportIBs::Parser eventReportIBs;
    AttributeReportIBs::Parser attributeReportIBs;
    System::PacketBufferTLVReader reader;

    reader.Init(std::move(aPayload));
    reader.Next();

    err = report.Init(reader);
    SuccessOrExit(err);

#if CHIP_CONFIG_IM_ENABLE_SCHEMA_CHECK
    err = report.CheckSchemaValidity();
    SuccessOrExit(err);
#endif

    err = report.GetSuppressResponse(&suppressResponse);
    if (CHIP_END_OF_TLV == err)
    {
        suppressResponse = false;
        err              = CHIP_NO_ERROR;
    }
    SuccessOrExit(err);

    err = report.GetSubscriptionId(&subscriptionId);
    if (CHIP_NO_ERROR == err)
    {
        if (mIsPrimingReports)
        {
            mSubscriptionId = subscriptionId;
        }
        else if (!IsMatchingClient(subscriptionId))
        {
            err = CHIP_ERROR_INVALID_ARGUMENT;
        }
    }
    else if (CHIP_END_OF_TLV == err)
    {
        if (IsSubscriptionType())
        {
            err = CHIP_ERROR_INVALID_ARGUMENT;
        }
        else
        {
            err = CHIP_NO_ERROR;
        }
    }
    SuccessOrExit(err);

    err = report.GetMoreChunkedMessages(&mPendingMoreChunks);
    if (CHIP_END_OF_TLV == err)
    {
        mPendingMoreChunks = false;
        err                = CHIP_NO_ERROR;
    }
    SuccessOrExit(err);

    err                   = report.GetEventReports(&eventReportIBs);
    isEventReportsPresent = (err == CHIP_NO_ERROR);
    if (err == CHIP_END_OF_TLV)
    {
        err = CHIP_NO_ERROR;
    }
    SuccessOrExit(err);

    if (isEventReportsPresent)
    {
        chip::TLV::TLVReader EventReportsReader;
        eventReportIBs.GetReader(&EventReportsReader);
        err = ProcessEventReportIBs(EventReportsReader);
        SuccessOrExit(err);
    }

    err                         = report.GetAttributeReportIBs(&attributeReportIBs);
    isAttributeReportIBsPresent = (err == CHIP_NO_ERROR);
    if (err == CHIP_END_OF_TLV)
    {
        err = CHIP_NO_ERROR;
    }
    SuccessOrExit(err);

    if (isAttributeReportIBsPresent)
    {
        TLV::TLVReader attributeReportIBsReader;
        attributeReportIBs.GetReader(&attributeReportIBsReader);

        if (mIsInitialReport)
        {
            mpCallback.OnReportBegin(this);
            mIsInitialReport = false;
        }

        err = ProcessAttributeReportIBs(attributeReportIBsReader);
        SuccessOrExit(err);

        if (!mPendingMoreChunks)
        {
            mpCallback.OnReportEnd(this);
            mIsInitialReport = true;
        }
    }

exit:
    if (IsSubscriptionType())
    {
        if (IsAwaitingInitialReport())
        {
            MoveToState(ClientState::AwaitingSubscribeResponse);
        }
        else
        {
            RefreshLivenessCheckTimer();
        }
    }

    if (!suppressResponse)
    {
        bool noResponseExpected = IsSubscriptionIdle() && !mPendingMoreChunks;
        err                     = StatusResponse::Send(err == CHIP_NO_ERROR ? Protocols::InteractionModel::Status::Success
                                                        : Protocols::InteractionModel::Status::InvalidSubscription,
                                   mpExchangeCtx, !noResponseExpected);

        if (noResponseExpected || (err != CHIP_NO_ERROR))
        {
            mpExchangeCtx = nullptr;
        }
    }

    mIsPrimingReports = false;
    return err;
}

void ReadClient::OnResponseTimeout(Messaging::ExchangeContext * apExchangeContext)
{
    ChipLogProgress(DataManagement, "Time out! failed to receive report data from Exchange: " ChipLogFormatExchange,
                    ChipLogValueExchange(apExchangeContext));
    Close(CHIP_ERROR_TIMEOUT);
}

CHIP_ERROR ReadClient::ProcessAttributePath(AttributePathIB::Parser & aAttributePathParser,
                                            ConcreteDataAttributePath & aAttributePath)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    // The ReportData must contain a concrete attribute path
    err = aAttributePathParser.GetEndpoint(&(aAttributePath.mEndpointId));
    VerifyOrReturnError(err == CHIP_NO_ERROR, CHIP_ERROR_IM_MALFORMED_ATTRIBUTE_PATH);
    err = aAttributePathParser.GetCluster(&(aAttributePath.mClusterId));
    VerifyOrReturnError(err == CHIP_NO_ERROR, CHIP_ERROR_IM_MALFORMED_ATTRIBUTE_PATH);
    err = aAttributePathParser.GetAttribute(&(aAttributePath.mAttributeId));
    VerifyOrReturnError(err == CHIP_NO_ERROR, CHIP_ERROR_IM_MALFORMED_ATTRIBUTE_PATH);

    DataModel::Nullable<ListIndex> listIndex;
    err = aAttributePathParser.GetListIndex(&(listIndex));
    if (CHIP_END_OF_TLV == err)
    {
        err = CHIP_NO_ERROR;
    }
    else if (listIndex.IsNull())
    {
        aAttributePath.mListOp = ConcreteDataAttributePath::ListOperation::AppendItem;
    }
    else
    {
        // TODO: Add ListOperation::ReplaceItem support. (Attribute path with valid list index)
        err = CHIP_ERROR_IM_MALFORMED_ATTRIBUTE_PATH;
    }
    VerifyOrReturnError(err == CHIP_NO_ERROR, CHIP_ERROR_IM_MALFORMED_ATTRIBUTE_PATH);
    return CHIP_NO_ERROR;
}

CHIP_ERROR ReadClient::ProcessAttributeReportIBs(TLV::TLVReader & aAttributeReportIBsReader)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    while (CHIP_NO_ERROR == (err = aAttributeReportIBsReader.Next()))
    {
        TLV::TLVReader dataReader;
        AttributeReportIB::Parser report;
        AttributeDataIB::Parser data;
        AttributeStatusIB::Parser status;
        AttributePathIB::Parser path;
        ConcreteDataAttributePath attributePath;
        StatusIB statusIB;

        TLV::TLVReader reader = aAttributeReportIBsReader;
        ReturnErrorOnFailure(report.Init(reader));

        err = report.GetAttributeStatus(&status);
        if (CHIP_NO_ERROR == err)
        {
            StatusIB::Parser errorStatus;
            ReturnErrorOnFailure(status.GetPath(&path));
            ReturnErrorOnFailure(ProcessAttributePath(path, attributePath));
            ReturnErrorOnFailure(status.GetErrorStatus(&errorStatus));
            ReturnErrorOnFailure(errorStatus.DecodeStatusIB(statusIB));
            mpCallback.OnAttributeData(this, attributePath, nullptr, statusIB);
        }
        else if (CHIP_END_OF_TLV == err)
        {
            ReturnErrorOnFailure(report.GetAttributeData(&data));
            ReturnErrorOnFailure(data.GetPath(&path));
            ReturnErrorOnFailure(ProcessAttributePath(path, attributePath));
            ReturnErrorOnFailure(data.GetData(&dataReader));

            // The element in an array may be another array -- so we should only set the list operation when we are handling the
            // whole list.
            if (!attributePath.IsListOperation() && dataReader.GetType() == TLV::kTLVType_Array)
            {
                attributePath.mListOp = ConcreteDataAttributePath::ListOperation::ReplaceAll;
            }

            mpCallback.OnAttributeData(this, attributePath, &dataReader, statusIB);
        }
    }

    if (CHIP_END_OF_TLV == err)
    {
        err = CHIP_NO_ERROR;
    }

    return err;
}

CHIP_ERROR ReadClient::ProcessEventReportIBs(TLV::TLVReader & aEventReportIBsReader)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    while (CHIP_NO_ERROR == (err = aEventReportIBsReader.Next()))
    {
        TLV::TLVReader dataReader;
        EventReportIB::Parser report;
        EventDataIB::Parser data;
        EventHeader header;

        TLV::TLVReader reader = aEventReportIBsReader;
        ReturnErrorOnFailure(report.Init(reader));

        ReturnErrorOnFailure(report.GetEventData(&data));

        header.mTimestamp = mEventTimestamp;
        ReturnErrorOnFailure(data.DecodeEventHeader(header));
        mEventTimestamp = header.mTimestamp;
        mEventMin       = header.mEventNumber + 1;
        ReturnErrorOnFailure(data.GetData(&dataReader));

        mpCallback.OnEventData(this, header, &dataReader, nullptr);
    }

    if (CHIP_END_OF_TLV == err)
    {
        err = CHIP_NO_ERROR;
    }

    return err;
}

CHIP_ERROR ReadClient::RefreshLivenessCheckTimer()
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    CancelLivenessCheckTimer();
    VerifyOrReturnError(mpExchangeCtx != nullptr, err = CHIP_ERROR_INCORRECT_STATE);
    VerifyOrReturnError(mpExchangeCtx->HasSessionHandle(), err = CHIP_ERROR_INCORRECT_STATE);

    System::Clock::Timeout timeout =
        System::Clock::Seconds16(mMaxIntervalCeilingSeconds) + mpExchangeCtx->GetSessionHandle()->GetAckTimeout();
    // EFR32/MBED/INFINION/K32W's chrono count return long unsinged, but other platform returns unsigned
    ChipLogProgress(DataManagement, "Refresh LivenessCheckTime with %lu milliseconds", static_cast<long unsigned>(timeout.count()));
    err = InteractionModelEngine::GetInstance()->GetExchangeManager()->GetSessionManager()->SystemLayer()->StartTimer(
        timeout, OnLivenessTimeoutCallback, this);

    if (err != CHIP_NO_ERROR)
    {
        Close(err);
    }

    return err;
}

void ReadClient::CancelLivenessCheckTimer()
{
    InteractionModelEngine::GetInstance()->GetExchangeManager()->GetSessionManager()->SystemLayer()->CancelTimer(
        OnLivenessTimeoutCallback, this);
}

void ReadClient::OnLivenessTimeoutCallback(System::Layer * apSystemLayer, void * apAppState)
{
    ReadClient * const client = reinterpret_cast<ReadClient *>(apAppState);

    //
    // Might as well try to see if this instance exists in the tracked list in the IM.
    // This might blow-up if either the client has since been free'ed (use-after-free), or if the engine has since
    // been shutdown at which point the client wouldn't exist in the active read client list.
    //
    VerifyOrDie(client->mpImEngine->InActiveReadClientList(client));

    ChipLogError(DataManagement, "Subscription Liveness timeout with peer node 0x%" PRIx64 ", shutting down ", client->mPeerNodeId);

    // TODO: add a more specific error here for liveness timeout failure to distinguish between other classes of timeouts (i.e
    // response timeouts).
    client->Close(CHIP_ERROR_TIMEOUT);
}

CHIP_ERROR ReadClient::ProcessSubscribeResponse(System::PacketBufferHandle && aPayload)
{
    System::PacketBufferTLVReader reader;
    reader.Init(std::move(aPayload));
    ReturnLogErrorOnFailure(reader.Next());

    SubscribeResponseMessage::Parser subscribeResponse;
    ReturnLogErrorOnFailure(subscribeResponse.Init(reader));

#if CHIP_CONFIG_IM_ENABLE_SCHEMA_CHECK
    ReturnLogErrorOnFailure(subscribeResponse.CheckSchemaValidity());
#endif

    uint64_t subscriptionId = 0;
    ReturnLogErrorOnFailure(subscribeResponse.GetSubscriptionId(&subscriptionId));
    VerifyOrReturnLogError(IsMatchingClient(subscriptionId), CHIP_ERROR_INVALID_ARGUMENT);
    ReturnLogErrorOnFailure(subscribeResponse.GetMinIntervalFloorSeconds(&mMinIntervalFloorSeconds));
    ReturnLogErrorOnFailure(subscribeResponse.GetMaxIntervalCeilingSeconds(&mMaxIntervalCeilingSeconds));

    mpCallback.OnSubscriptionEstablished(this);

    MoveToState(ClientState::SubscriptionActive);

    RefreshLivenessCheckTimer();

    return CHIP_NO_ERROR;
}

CHIP_ERROR ReadClient::SendSubscribeRequest(ReadPrepareParams & aReadPrepareParams)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    System::PacketBufferHandle msgBuf;
    System::PacketBufferTLVWriter writer;
    SubscribeRequestMessage::Builder request;

    VerifyOrReturnError(ClientState::Idle == mState, err = CHIP_ERROR_INCORRECT_STATE);

    msgBuf = System::PacketBufferHandle::New(kMaxSecureSduLengthBytes);
    VerifyOrReturnError(!msgBuf.IsNull(), err = CHIP_ERROR_NO_MEMORY);

    VerifyOrReturnError(aReadPrepareParams.mMinIntervalFloorSeconds <= aReadPrepareParams.mMaxIntervalCeilingSeconds,
                        err = CHIP_ERROR_INVALID_ARGUMENT);

    writer.Init(std::move(msgBuf));

    ReturnErrorOnFailure(request.Init(&writer));

    request.KeepSubscriptions(aReadPrepareParams.mKeepSubscriptions)
        .MinIntervalFloorSeconds(aReadPrepareParams.mMinIntervalFloorSeconds)
        .MaxIntervalCeilingSeconds(aReadPrepareParams.mMaxIntervalCeilingSeconds);

    if (aReadPrepareParams.mAttributePathParamsListSize != 0 && aReadPrepareParams.mpAttributePathParamsList != nullptr)
    {
        AttributePathIBs::Builder & attributePathListBuilder = request.CreateAttributeRequests();
        ReturnErrorOnFailure(err = attributePathListBuilder.GetError());
        ReturnErrorOnFailure(GenerateAttributePathList(attributePathListBuilder, aReadPrepareParams.mpAttributePathParamsList,
                                                       aReadPrepareParams.mAttributePathParamsListSize));
    }

    if (aReadPrepareParams.mEventPathParamsListSize != 0 && aReadPrepareParams.mpEventPathParamsList != nullptr)
    {
        EventPathIBs::Builder & eventPathListBuilder = request.CreateEventRequests();
        ReturnErrorOnFailure(err = eventPathListBuilder.GetError());
        ReturnErrorOnFailure(GenerateEventPaths(eventPathListBuilder, aReadPrepareParams.mpEventPathParamsList,
                                                aReadPrepareParams.mEventPathParamsListSize));

        if (aReadPrepareParams.mEventNumber != 0)
        {
            mEventMin = aReadPrepareParams.mEventNumber;
        }

        EventFilterIBs::Builder & eventFilters = request.CreateEventFilters();
        ReturnErrorOnFailure(err = request.GetError());
        EventFilterIB::Builder & eventFilter = eventFilters.CreateEventFilter();
        ReturnErrorOnFailure(err = eventFilters.GetError());
        eventFilter.EventMin(mEventMin).EndOfEventFilterIB();
        ReturnErrorOnFailure(err = eventFilter.GetError());
        eventFilters.EndOfEventFilters();
        ReturnErrorOnFailure(err = eventFilters.GetError());
    }

    request.IsFabricFiltered(aReadPrepareParams.mIsFabricFiltered).EndOfSubscribeRequestMessage();
    ReturnErrorOnFailure(err = request.GetError());

    ReturnErrorOnFailure(writer.Finalize(&msgBuf));

    mpExchangeCtx = mpExchangeMgr->NewContext(aReadPrepareParams.mSessionHolder.Get(), this);
    VerifyOrReturnError(mpExchangeCtx != nullptr, err = CHIP_ERROR_NO_MEMORY);
    mpExchangeCtx->SetResponseTimeout(kImMessageTimeout);

    ReturnErrorOnFailure(mpExchangeCtx->SendMessage(Protocols::InteractionModel::MsgType::SubscribeRequest, std::move(msgBuf),
                                                    Messaging::SendFlags(Messaging::SendMessageFlags::kExpectResponse)));

    mPeerNodeId  = aReadPrepareParams.mSessionHolder->AsSecureSession()->GetPeerNodeId();
    mFabricIndex = aReadPrepareParams.mSessionHolder->AsSecureSession()->GetFabricIndex();

    MoveToState(ClientState::AwaitingInitialReport);

    return CHIP_NO_ERROR;
}

}; // namespace app
}; // namespace chip
