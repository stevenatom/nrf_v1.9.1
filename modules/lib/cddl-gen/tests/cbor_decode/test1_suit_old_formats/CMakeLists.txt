#
# Copyright (c) 2020 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: Apache-2.0
#

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(NONE)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE
  ${app_sources})

set(py_command3
  cddl-gen
  -c ${CMAKE_CURRENT_LIST_DIR}/../../cases/manifest-moran3.cddl
  --default-max-qty 6
  code
  --output-cmake ${PROJECT_BINARY_DIR}/manifest-moran3.cmake
  -t OuterWrapper
  --decode
  )

set(py_command4
  cddl-gen
  -c ${CMAKE_CURRENT_LIST_DIR}/../../cases/manifest-moran4.cddl
  --default-max-qty 6
  code
  --output-cmake ${PROJECT_BINARY_DIR}/manifest-moran4.cmake
  -t SUIT_Outer_Wrapper
  --decode
  )

execute_process(
  COMMAND
  ${py_command3}
  COMMAND
  ${py_command4}
  COMMAND_ERROR_IS_FATAL ANY
)


include(${PROJECT_BINARY_DIR}/manifest-moran3.cmake)
include(${PROJECT_BINARY_DIR}/manifest-moran4.cmake)

target_link_libraries(manifest-moran3 PRIVATE zephyr_interface)
target_link_libraries(manifest-moran4 PRIVATE zephyr_interface)
target_link_libraries(app PRIVATE manifest-moran3 manifest-moran4)

zephyr_compile_options(-Werror)
