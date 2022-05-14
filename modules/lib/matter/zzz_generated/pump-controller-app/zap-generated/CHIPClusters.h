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

// Prevent multiple inclusion
#pragma once

#include <app-common/zap-generated/ids/Clusters.h>
#include <app-common/zap-generated/ids/Commands.h>

#include <controller/CHIPCluster.h>
#include <lib/core/CHIPCallback.h>
#include <lib/support/Span.h>

namespace chip {
namespace Controller {

class DLL_EXPORT FlowMeasurementCluster : public ClusterBase
{
public:
    FlowMeasurementCluster() : ClusterBase(app::Clusters::FlowMeasurement::Id) {}
    ~FlowMeasurementCluster() {}
};

class DLL_EXPORT LevelControlCluster : public ClusterBase
{
public:
    LevelControlCluster() : ClusterBase(app::Clusters::LevelControl::Id) {}
    ~LevelControlCluster() {}

    // Cluster Commands
    CHIP_ERROR Move(Callback::Cancelable * onSuccessCallback, Callback::Cancelable * onFailureCallback, uint8_t moveMode,
                    uint8_t rate, uint8_t optionMask, uint8_t optionOverride);
    CHIP_ERROR MoveToLevel(Callback::Cancelable * onSuccessCallback, Callback::Cancelable * onFailureCallback, uint8_t level,
                           uint16_t transitionTime, uint8_t optionMask, uint8_t optionOverride);
    CHIP_ERROR MoveToLevelWithOnOff(Callback::Cancelable * onSuccessCallback, Callback::Cancelable * onFailureCallback,
                                    uint8_t level, uint16_t transitionTime);
    CHIP_ERROR MoveWithOnOff(Callback::Cancelable * onSuccessCallback, Callback::Cancelable * onFailureCallback, uint8_t moveMode,
                             uint8_t rate);
    CHIP_ERROR Step(Callback::Cancelable * onSuccessCallback, Callback::Cancelable * onFailureCallback, uint8_t stepMode,
                    uint8_t stepSize, uint16_t transitionTime, uint8_t optionMask, uint8_t optionOverride);
    CHIP_ERROR StepWithOnOff(Callback::Cancelable * onSuccessCallback, Callback::Cancelable * onFailureCallback, uint8_t stepMode,
                             uint8_t stepSize, uint16_t transitionTime);
    CHIP_ERROR Stop(Callback::Cancelable * onSuccessCallback, Callback::Cancelable * onFailureCallback, uint8_t optionMask,
                    uint8_t optionOverride);
    CHIP_ERROR StopWithOnOff(Callback::Cancelable * onSuccessCallback, Callback::Cancelable * onFailureCallback);
};

class DLL_EXPORT OnOffCluster : public ClusterBase
{
public:
    OnOffCluster() : ClusterBase(app::Clusters::OnOff::Id) {}
    ~OnOffCluster() {}

    // Cluster Commands
    CHIP_ERROR Off(Callback::Cancelable * onSuccessCallback, Callback::Cancelable * onFailureCallback);
    CHIP_ERROR On(Callback::Cancelable * onSuccessCallback, Callback::Cancelable * onFailureCallback);
    CHIP_ERROR Toggle(Callback::Cancelable * onSuccessCallback, Callback::Cancelable * onFailureCallback);
};

class DLL_EXPORT PressureMeasurementCluster : public ClusterBase
{
public:
    PressureMeasurementCluster() : ClusterBase(app::Clusters::PressureMeasurement::Id) {}
    ~PressureMeasurementCluster() {}
};

class DLL_EXPORT PumpConfigurationAndControlCluster : public ClusterBase
{
public:
    PumpConfigurationAndControlCluster() : ClusterBase(app::Clusters::PumpConfigurationAndControl::Id) {}
    ~PumpConfigurationAndControlCluster() {}
};

class DLL_EXPORT TemperatureMeasurementCluster : public ClusterBase
{
public:
    TemperatureMeasurementCluster() : ClusterBase(app::Clusters::TemperatureMeasurement::Id) {}
    ~TemperatureMeasurementCluster() {}
};

} // namespace Controller
} // namespace chip
