/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
 *    Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
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

#include "AppTask.h"
#include "AppConfig.h"
#include "AppEvent.h"
#include "ButtonHandler.h"
#include "LEDWidget.h"
#include "qrcodegen.h"
#include <app-common/zap-generated/attribute-id.h>
#include <app-common/zap-generated/attribute-type.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/cluster-id.h>
#include <app/server/Dnssd.h>
#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>
#include <app/util/attribute-storage.h>
#include <assert.h>
#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include <cy_wcm.h>
#include <platform/CHIPDeviceLayer.h>
#include <setup_payload/QRCodeSetupPayloadGenerator.h>
#include <setup_payload/SetupPayload.h>

#define APP_EVENT_QUEUE_SIZE 10
#define APP_TASK_STACK_SIZE (4096)
#define APP_WAIT_LOOP 1000

LEDWidget sStatusLED;
LEDWidget sLightLED;
LEDWidget sClusterLED;

namespace {

TaskHandle_t sAppTaskHandle;
QueueHandle_t sAppEventQueue;

uint8_t sAppEventQueueBuffer[APP_EVENT_QUEUE_SIZE * sizeof(AppEvent)];
StaticQueue_t sAppEventQueueStruct;

StackType_t appStack[APP_TASK_STACK_SIZE / sizeof(StackType_t)];
StaticTask_t appTaskStruct;
} // namespace

using namespace chip::TLV;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;
using namespace ::chip::app::Clusters;

AppTask AppTask::sAppTask;

CHIP_ERROR AppTask::StartAppTask()
{
    sAppEventQueue = xQueueCreateStatic(APP_EVENT_QUEUE_SIZE, sizeof(AppEvent), sAppEventQueueBuffer, &sAppEventQueueStruct);
    if (sAppEventQueue == NULL)
    {
        P6_LOG("Failed to allocate app event queue");
        appError(APP_ERROR_EVENT_QUEUE_FAILED);
    }
    // Start App task.
    sAppTaskHandle = xTaskCreateStatic(AppTaskMain, APP_TASK_NAME, ArraySize(appStack), NULL, 1, appStack, &appTaskStruct);
    return (sAppTaskHandle == nullptr) ? APP_ERROR_CREATE_TASK_FAILED : CHIP_NO_ERROR;
}

CHIP_ERROR AppTask::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    // Register the callback to init the MDNS server when connectivity is available
    PlatformMgr().AddEventHandler(
        [](const ChipDeviceEvent * event, intptr_t arg) {
            // Restart the server whenever an ip address is renewed
            if (event->Type == DeviceEventType::kInternetConnectivityChange)
            {
                if (event->InternetConnectivityChange.IPv4 == kConnectivity_Established ||
                    event->InternetConnectivityChange.IPv6 == kConnectivity_Established)
                {
                    chip::app::DnssdServer::Instance().StartServer();
                }
            }
        },
        0);
    // Init ZCL Data Model
    chip::Server::GetInstance().Init();

    // Initialize device attestation config
    SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());

    // Initialise WSTK buttons PB0 and PB1 (including debounce).
    ButtonHandler::Init();

    P6_LOG("Current Software Version: %s", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING);

    // Initialize LEDs
    sStatusLED.Init(SYSTEM_STATE_LED);
    sLightLED.Init(LIGHT_LED);
    sClusterLED.RGB_init();

    ConfigurationMgr().LogDeviceConfig();

    // Print setup info
    PrintOnboardingCodes(chip::RendezvousInformationFlag(chip::RendezvousInformationFlag::kBLE));

    return err;
}

void AppTask::AppTaskMain(void * pvParameter)
{
    AppEvent event;
    CHIP_ERROR err = sAppTask.Init();
    if (err != CHIP_NO_ERROR)
    {
        P6_LOG("AppTask.Init() failed");
        appError(err);
    }

    P6_LOG("App Task started");

    while (true)
    {
        BaseType_t eventReceived = xQueueReceive(sAppEventQueue, &event, pdMS_TO_TICKS(10));
        while (eventReceived == pdTRUE)
        {
            sAppTask.DispatchEvent(&event);
            eventReceived = xQueueReceive(sAppEventQueue, &event, 0);
        }
    }
}

void AppTask::LightActionEventHandler(AppEvent * aEvent)
{
    /* ON/OFF Light Led based on Button interrupt */
    sLightLED.Invert();

    /* Update OnOff Cluster state */
    sAppTask.OnOffUpdateClusterState();
}

void AppTask::ButtonEventHandler(uint8_t btnIdx, uint8_t btnAction)
{
    if (btnIdx != APP_LIGHT_BUTTON_IDX)
    {
        return;
    }

    AppEvent button_event              = {};
    button_event.Type                  = AppEvent::kEventType_Button;
    button_event.ButtonEvent.ButtonIdx = btnIdx;
    button_event.ButtonEvent.Action    = btnAction;

    if ((btnIdx == APP_LIGHT_BUTTON_IDX) && (btnAction == APP_BUTTON_RELEASED))
    {
        button_event.Handler = LightActionEventHandler;
        sAppTask.PostEvent(&button_event);
    }
}

void AppTask::PostEvent(const AppEvent * aEvent)
{
    if (sAppEventQueue != NULL)
    {
        BaseType_t status;
        if (xPortIsInsideInterrupt())
        {
            BaseType_t higherPrioTaskWoken = pdFALSE;
            status                         = xQueueSendFromISR(sAppEventQueue, aEvent, &higherPrioTaskWoken);

#ifdef portYIELD_FROM_ISR
            portYIELD_FROM_ISR(higherPrioTaskWoken);
#elif portEND_SWITCHING_ISR // portYIELD_FROM_ISR or portEND_SWITCHING_ISR
            portEND_SWITCHING_ISR(higherPrioTaskWoken);
#else                       // portYIELD_FROM_ISR or portEND_SWITCHING_ISR
#error "Must have portYIELD_FROM_ISR or portEND_SWITCHING_ISR"
#endif // portYIELD_FROM_ISR or portEND_SWITCHING_ISR
        }
        else
        {
            status = xQueueSend(sAppEventQueue, aEvent, 1);
        }

        if (!status)
            P6_LOG("Failed to post event to app task event queue");
    }
    else
    {
        P6_LOG("Event Queue is NULL should never happen");
    }
}

void AppTask::DispatchEvent(AppEvent * aEvent)
{
    if (aEvent->Handler)
    {
        aEvent->Handler(aEvent);
    }
    else
    {
        P6_LOG("Event received with no handler. Dropping event.");
    }
}

void AppTask::OnOffUpdateClusterState(void)
{
    uint8_t newValue = sLightLED.Get();

    // write the new on/off value
    EmberAfStatus status = OnOff::Attributes::OnOff::Set(2, newValue);
    if (status != EMBER_ZCL_STATUS_SUCCESS)
    {
        P6_LOG("ERR: updating on/off %x", status);
    }
}

bool lowPowerClusterSleep()
{
    return true;
}
