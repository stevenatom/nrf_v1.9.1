#!/bin/bash

cmake -GNinja -DBOARD=$1 ${ZEPHYR_BASE}/../bootloader/mcuboot/boot/zephyr -DCONFIG_MCUBOOT_SERIAL=y -DCONFIG_UART_CONSOLE=n -DCONFIG_BOOT_SERIAL_DETECT_PIN_VAL=1 -DCONFIG_HW_STACK_PROTECTION=y -DCONFIG_CONSOLE_HANDLER=n
ninja
