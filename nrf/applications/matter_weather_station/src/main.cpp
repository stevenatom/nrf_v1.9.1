/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "app_task.h"

#include <logging/log.h>

#ifdef CONFIG_USB_DEVICE_STACK
#include <usb/usb_device.h>
#endif

#include <lib/support/CHIPMem.h>
#include <platform/CHIPDeviceLayer.h>
#include <system/SystemError.h>

LOG_MODULE_REGISTER(app);

using namespace ::chip::DeviceLayer;

int main()
{
	int ret = 0;
	CHIP_ERROR err = CHIP_NO_ERROR;

#ifdef CONFIG_USB_DEVICE_STACK
	err = chip::System::MapErrorZephyr(usb_enable(NULL));
	if (err != CHIP_NO_ERROR) {
		goto exit;
	}
#endif

	err = chip::Platform::MemoryInit();
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("Platform::MemoryInit() failed");
		goto exit;
	}

	LOG_INF("Init CHIP stack");
	err = PlatformMgr().InitChipStack();
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("PlatformMgr().InitChipStack() failed");
		goto exit;
	}

	LOG_INF("Starting CHIP task");
	err = PlatformMgr().StartEventLoopTask();
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("PlatformMgr().StartEventLoopTask() failed");
		goto exit;
	}

	LOG_INF("Init Thread stack");
	err = ThreadStackMgr().InitThreadStack();
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("ThreadStackMgr().InitThreadStack() failed");
		goto exit;
	}

#ifdef CONFIG_OPENTHREAD_MTD_SED
	err = ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_SleepyEndDevice);
#else
	err = ConnectivityMgr().SetThreadDeviceType(ConnectivityManager::kThreadDeviceType_MinimalEndDevice);
#endif
	if (err != CHIP_NO_ERROR) {
		LOG_ERR("ConnectivityMgr().SetThreadDeviceType() failed");
		goto exit;
	}

	ret = GetAppTask().StartApp();
	if (ret != 0) {
		err = chip::System::MapErrorZephyr(ret);
	}

exit:
	return err == CHIP_NO_ERROR ? EXIT_SUCCESS : EXIT_FAILURE;
}
