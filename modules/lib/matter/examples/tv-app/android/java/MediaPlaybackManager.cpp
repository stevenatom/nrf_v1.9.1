/**
 *
 *    Copyright (c) 2021 Project CHIP Authors
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

#include "MediaPlaybackManager.h"
#include "TvApp-JNI.h"
#include <app-common/zap-generated/ids/Clusters.h>
#include <cstdint>
#include <jni.h>
#include <lib/support/CHIPJNIError.h>
#include <lib/support/JniReferences.h>
#include <lib/support/JniTypeWrappers.h>

#include "MediaPlaybackManager.h"

using namespace chip;
using namespace chip::app::Clusters::MediaPlayback;

/** @brief Media PlayBack Cluster Init
 *
 * This function is called when a specific cluster is initialized. It gives the
 * application an opportunity to take care of cluster initialization procedures.
 * It is called exactly once for each endpoint where cluster is present.
 *
 * @param endpoint   Ver.: always
 *
 */
void emberAfMediaPlaybackClusterInitCallback(chip::EndpointId endpoint)
{
    ChipLogProgress(Zcl, "TV Android App: MediaPlayback::PostClusterInit");
    TvAppJNIMgr().PostClusterInit(chip::app::Clusters::MediaPlayback::Id, endpoint);
}

void MediaPlaybackManager::NewManager(jint endpoint, jobject manager)
{
    ChipLogProgress(Zcl, "TV Android App: MediaPlayback::SetDefaultDelegate");
    MediaPlaybackManager * mgr = new MediaPlaybackManager();
    mgr->InitializeWithObjects(manager);
    chip::app::Clusters::MediaPlayback::SetDefaultDelegate(static_cast<EndpointId>(endpoint), mgr);
}

PlaybackStateEnum MediaPlaybackManager::HandleGetCurrentState()
{
    uint64_t ret = HandleMediaRequestGetAttribute(MEDIA_PLAYBACK_ATTRIBUTE_PLAYBACK_STATE);
    return static_cast<PlaybackStateEnum>(ret);
}

uint64_t MediaPlaybackManager::HandleGetStartTime()
{
    return HandleMediaRequestGetAttribute(MEDIA_PLAYBACK_ATTRIBUTE_START_TIME);
}

uint64_t MediaPlaybackManager::HandleGetDuration()
{
    return HandleMediaRequestGetAttribute(MEDIA_PLAYBACK_ATTRIBUTE_DURATION);
}

float MediaPlaybackManager::HandleGetPlaybackSpeed()
{
    uint64_t ret = HandleMediaRequestGetAttribute(MEDIA_PLAYBACK_ATTRIBUTE_SPEED);
    return static_cast<float>(ret) / 10000.0f;
}

uint64_t MediaPlaybackManager::HandleGetSeekRangeStart()
{
    return HandleMediaRequestGetAttribute(MEDIA_PLAYBACK_ATTRIBUTE_SEEK_RANGE_START);
}

uint64_t MediaPlaybackManager::HandleGetSeekRangeEnd()
{
    return HandleMediaRequestGetAttribute(MEDIA_PLAYBACK_ATTRIBUTE_SEEK_RANGE_END);
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandlePlay()
{
    return HandleMediaRequest(MEDIA_PLAYBACK_REQUEST_PLAY, 0);
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandlePause()
{
    return HandleMediaRequest(MEDIA_PLAYBACK_REQUEST_PAUSE, 0);
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandleStop()
{
    return HandleMediaRequest(MEDIA_PLAYBACK_REQUEST_STOP, 0);
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandleFastForward()
{
    return HandleMediaRequest(MEDIA_PLAYBACK_REQUEST_FAST_FORWARD, 0);
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandlePrevious()
{
    return HandleMediaRequest(MEDIA_PLAYBACK_REQUEST_PREVIOUS, 0);
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandleRewind()
{
    return HandleMediaRequest(MEDIA_PLAYBACK_REQUEST_REWIND, 0);
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandleSkipBackward(const uint64_t & deltaPositionMilliseconds)
{
    return HandleMediaRequest(MEDIA_PLAYBACK_REQUEST_SKIP_BACKWARD, deltaPositionMilliseconds);
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandleSkipForward(const uint64_t & deltaPositionMilliseconds)
{
    return HandleMediaRequest(MEDIA_PLAYBACK_REQUEST_SKIP_FORWARD, deltaPositionMilliseconds);
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandleSeekRequest(const uint64_t & positionMilliseconds)
{
    return HandleMediaRequest(MEDIA_PLAYBACK_REQUEST_SEEK, positionMilliseconds);
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandleNext()
{
    return HandleMediaRequest(MEDIA_PLAYBACK_REQUEST_NEXT, 0);
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandleStartOverRequest()
{
    return HandleMediaRequest(MEDIA_PLAYBACK_REQUEST_START_OVER, 0);
}

void MediaPlaybackManager::InitializeWithObjects(jobject managerObject)
{
    JNIEnv * env = JniReferences::GetInstance().GetEnvForCurrentThread();
    VerifyOrReturn(env != nullptr, ChipLogError(Zcl, "Failed to GetEnvForCurrentThread for MediaPlaybackManager"));

    mMediaPlaybackManagerObject = env->NewGlobalRef(managerObject);
    VerifyOrReturn(mMediaPlaybackManagerObject != nullptr, ChipLogError(Zcl, "Failed to NewGlobalRef MediaPlaybackManager"));

    jclass mMediaPlaybackManagerClass = env->GetObjectClass(managerObject);
    VerifyOrReturn(mMediaPlaybackManagerClass != nullptr, ChipLogError(Zcl, "Failed to get MediaPlaybackManager Java class"));

    mGetAttributeMethod = env->GetMethodID(mMediaPlaybackManagerClass, "getAttributes", "(I)J");
    if (mGetAttributeMethod == nullptr)
    {
        ChipLogError(Zcl, "Failed to access MediaPlaybackManager 'getAttributes' method");
        env->ExceptionClear();
    }

    mRequestMethod = env->GetMethodID(mMediaPlaybackManagerClass, "request", "(IJ)I");
    if (mRequestMethod == nullptr)
    {
        ChipLogError(Zcl, "Failed to access MediaPlaybackManager 'request' method");
        env->ExceptionClear();
    }

    mGetPositionMethod =
        env->GetMethodID(mMediaPlaybackManagerClass, "getPosition", "()[Lcom/tcl/chip/tvapp/MediaPlaybackPosition;");
    if (mGetPositionMethod == nullptr)
    {
        ChipLogError(Zcl, "Failed to access MediaPlaybackManager 'getPosition' method");
        env->ExceptionClear();
    }
}

uint64_t MediaPlaybackManager::HandleMediaRequestGetAttribute(MediaPlaybackRequestAttribute attribute)
{
    uint64_t ret          = std::numeric_limits<uint64_t>::max();
    jlong jAttributeValue = -1;
    CHIP_ERROR err        = CHIP_NO_ERROR;
    JNIEnv * env          = JniReferences::GetInstance().GetEnvForCurrentThread();

    ChipLogProgress(Zcl, "Received MediaPlaybackManager::HandleMediaRequestGetAttribute:%d", attribute);
    VerifyOrExit(mMediaPlaybackManagerObject != nullptr, err = CHIP_ERROR_INCORRECT_STATE);
    VerifyOrExit(mGetAttributeMethod != nullptr, err = CHIP_ERROR_INCORRECT_STATE);
    VerifyOrExit(env != NULL, err = CHIP_JNI_ERROR_NO_ENV);

    jAttributeValue = env->CallLongMethod(mMediaPlaybackManagerObject, mGetAttributeMethod, static_cast<jint>(attribute));
    if (env->ExceptionCheck())
    {
        ChipLogError(AppServer, "Java exception in MediaPlaybackManager::GetAttribute");
        env->ExceptionDescribe();
        env->ExceptionClear();
        goto exit;
    }

    if (jAttributeValue >= 0)
    {
        ret = static_cast<uint64_t>(jAttributeValue);
    }
    else
    {
        err = CHIP_ERROR_INCORRECT_STATE;
    }

exit:
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(Zcl, "MediaPlaybackManager::GetAttribute status error: %s", err.AsString());
    }

    return ret;
}

Commands::PlaybackResponse::Type MediaPlaybackManager::HandleMediaRequest(MediaPlaybackRequest mediaPlaybackRequest,
                                                                          uint64_t deltaPositionMilliseconds)

{
    Commands::PlaybackResponse::Type response;

    jint ret       = -1;
    CHIP_ERROR err = CHIP_NO_ERROR;
    JNIEnv * env   = JniReferences::GetInstance().GetEnvForCurrentThread();

    ChipLogProgress(Zcl, "MediaPlaybackManager::Request %d-%ld", mediaPlaybackRequest,
                    static_cast<long>(deltaPositionMilliseconds));
    VerifyOrExit(mMediaPlaybackManagerObject != nullptr, err = CHIP_ERROR_INCORRECT_STATE);
    VerifyOrExit(mRequestMethod != nullptr, err = CHIP_ERROR_INCORRECT_STATE);
    VerifyOrExit(env != NULL, err = CHIP_JNI_ERROR_NO_ENV);

    env->ExceptionClear();
    ret = env->CallIntMethod(mMediaPlaybackManagerObject, mRequestMethod, static_cast<jint>(mediaPlaybackRequest),
                             static_cast<jlong>(deltaPositionMilliseconds));
    if (env->ExceptionCheck())
    {
        ChipLogError(AppServer, "Java exception in MediaPlaybackManager::Request %d", mediaPlaybackRequest);
        env->ExceptionDescribe();
        env->ExceptionClear();
        response.status = StatusEnum::kInvalidStateForCommand;
    }
    response.status = static_cast<StatusEnum>(ret);

exit:
    if (err != CHIP_NO_ERROR)
    {
        response.status = StatusEnum::kInvalidStateForCommand;
        ChipLogError(Zcl, "MediaPlaybackManager::HandleMediaRequest status error: %s", err.AsString());
    }

    return response;
}

Structs::PlaybackPosition::Type MediaPlaybackManager::HandleGetSampledPosition()
{
    Structs::PlaybackPosition::Type response;
    response.updatedAt = 0;
    response.position  = 0;

    jobject positionObj;
    CHIP_ERROR err = CHIP_NO_ERROR;
    JNIEnv * env   = JniReferences::GetInstance().GetEnvForCurrentThread();

    ChipLogProgress(Zcl, "MediaPlaybackManager::HandleGetSampledPosition");
    VerifyOrExit(mMediaPlaybackManagerObject != nullptr, err = CHIP_ERROR_INCORRECT_STATE);
    VerifyOrExit(mGetPositionMethod != nullptr, err = CHIP_ERROR_INCORRECT_STATE);
    VerifyOrExit(env != NULL, err = CHIP_JNI_ERROR_NO_ENV);

    env->ExceptionClear();
    positionObj = env->CallObjectMethod(mMediaPlaybackManagerObject, mGetPositionMethod);
    if (env->ExceptionCheck())
    {
        ChipLogError(AppServer, "Java exception in MediaPlaybackManager::HandleGetSampledPosition");
        env->ExceptionDescribe();
        env->ExceptionClear();
        goto exit;
    }

    {
        jclass inputClass    = env->GetObjectClass(positionObj);
        jfieldID positionId  = env->GetFieldID(inputClass, "position", "J");
        jfieldID updatedAtId = env->GetFieldID(inputClass, "updatedAt", "J");
        response.position    = static_cast<uint64_t>(env->GetIntField(positionObj, positionId));
        response.updatedAt   = static_cast<uint64_t>(env->GetIntField(positionObj, updatedAtId));
    }

exit:
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(Zcl, "MediaPlaybackManager::GetAttribute status error: %s", err.AsString());
    }

    return response;
}