/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
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

// THIS FILE IS GENERATED BY ZAP

#pragma once

#include <app-common/zap-generated/af-structs.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app/InteractionModelEngine.h>
#include <app/data-model/DecodableList.h>
#include <app/util/af-enums.h>
#include <app/util/attribute-filter.h>
#include <app/util/im-client-callbacks.h>
#include <inttypes.h>
#include <lib/support/FunctionTraits.h>
#include <lib/support/Span.h>

// Note: The IMDefaultResponseCallback is a bridge to the old CallbackMgr before IM is landed, so it still accepts EmberAfStatus
// instead of IM status code.
// #6308 should handle IM error code on the application side, either modify this function or remove this.

// Cluster Specific Response Callbacks
typedef void (*GeneralCommissioningClusterArmFailSafeResponseCallback)(void * context, uint8_t errorCode, chip::CharSpan debugText);
typedef void (*GeneralCommissioningClusterCommissioningCompleteResponseCallback)(void * context, uint8_t errorCode,
                                                                                 chip::CharSpan debugText);
typedef void (*GeneralCommissioningClusterSetRegulatoryConfigResponseCallback)(void * context, uint8_t errorCode,
                                                                               chip::CharSpan debugText);
typedef void (*NetworkCommissioningClusterConnectNetworkResponseCallback)(void * context, uint8_t NetworkingStatus,
                                                                          chip::CharSpan DebugText, int32_t ErrorValue);
typedef void (*NetworkCommissioningClusterNetworkConfigResponseCallback)(void * context, uint8_t NetworkingStatus,
                                                                         chip::CharSpan DebugText);
typedef void (*NetworkCommissioningClusterScanNetworksResponseCallback)(
    void * context, uint8_t NetworkingStatus, chip::CharSpan DebugText,
    /* TYPE WARNING: array array defaults to */ uint8_t * WiFiScanResults,
    /* TYPE WARNING: array array defaults to */ uint8_t * ThreadScanResults);
typedef void (*OperationalCredentialsClusterAttestationResponseCallback)(void * context, chip::ByteSpan AttestationElements,
                                                                         chip::ByteSpan Signature);
typedef void (*OperationalCredentialsClusterCertificateChainResponseCallback)(void * context, chip::ByteSpan Certificate);
typedef void (*OperationalCredentialsClusterNOCResponseCallback)(void * context, uint8_t StatusCode, uint8_t FabricIndex,
                                                                 chip::CharSpan DebugText);
typedef void (*OperationalCredentialsClusterOpCSRResponseCallback)(void * context, chip::ByteSpan NOCSRElements,
                                                                   chip::ByteSpan AttestationSignature);

// List specific responses
void GeneralCommissioningClusterBasicCommissioningInfoListListAttributeFilter(chip::TLV::TLVReader * data,
                                                                              chip::Callback::Cancelable * onSuccessCallback,
                                                                              chip::Callback::Cancelable * onFailureCallback);
typedef void (*GeneralCommissioningBasicCommissioningInfoListListAttributeCallback)(
    void * context,
    const chip::app::DataModel::DecodableList<
        chip::app::Clusters::GeneralCommissioning::Structs::BasicCommissioningInfoType::DecodableType> & data);
void NetworkCommissioningClusterNetworksListAttributeFilter(chip::TLV::TLVReader * data,
                                                            chip::Callback::Cancelable * onSuccessCallback,
                                                            chip::Callback::Cancelable * onFailureCallback);
typedef void (*NetworkCommissioningNetworksListAttributeCallback)(
    void * context,
    const chip::app::DataModel::DecodableList<chip::app::Clusters::NetworkCommissioning::Structs::NetworkInfo::DecodableType> &
        data);
void OperationalCredentialsClusterNOCsListAttributeFilter(chip::TLV::TLVReader * data,
                                                          chip::Callback::Cancelable * onSuccessCallback,
                                                          chip::Callback::Cancelable * onFailureCallback);
typedef void (*OperationalCredentialsNOCsListAttributeCallback)(
    void * context,
    const chip::app::DataModel::DecodableList<chip::app::Clusters::OperationalCredentials::Structs::NOCStruct::DecodableType> &
        data);
void OperationalCredentialsClusterFabricsListListAttributeFilter(chip::TLV::TLVReader * data,
                                                                 chip::Callback::Cancelable * onSuccessCallback,
                                                                 chip::Callback::Cancelable * onFailureCallback);
typedef void (*OperationalCredentialsFabricsListListAttributeCallback)(
    void * context,
    const chip::app::DataModel::DecodableList<
        chip::app::Clusters::OperationalCredentials::Structs::FabricDescriptor::DecodableType> & data);
void OperationalCredentialsClusterTrustedRootCertificatesListAttributeFilter(chip::TLV::TLVReader * data,
                                                                             chip::Callback::Cancelable * onSuccessCallback,
                                                                             chip::Callback::Cancelable * onFailureCallback);
typedef void (*OperationalCredentialsTrustedRootCertificatesListAttributeCallback)(
    void * context, const chip::app::DataModel::DecodableList<chip::ByteSpan> & data);
