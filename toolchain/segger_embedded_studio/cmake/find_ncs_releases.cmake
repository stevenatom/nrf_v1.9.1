cmake_minimum_required(VERSION 3.5)

# The following is a list of all variables used for automatic toolchain selection and setup
set(PACKAGE_INFO_VARIABLES "ZEPHYR_BASE"
                           "ENV{ZEPHYR_BASE}"
                           "Ncs_DIR"
                           "NcsToolchain_DIR"
                           "NcsToolchain_FOUND"
                           "NCS_TOOLCHAIN_COMP_LIST"
                           "NORDIC_BOARDS"
                           "THINGY_BOARDS"
                           "OTHER_BOARDS"
)
set(NCS_INFO_JSON_STR "")

macro(clear_package_info)
  foreach(variable ${PACKAGE_INFO_VARIABLES})
    unset(${variable} CACHE)
    unset(${variable})
  endforeach()
endmacro()

macro(append_ncs_package_info)
  set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}{\"ncs_version\":\"${NCS_VERSION}\", \"ncs_path\":\"${NCS_DIR}\", \"nrf_path\":\"${NRF_DIR}\", \"zephyr_base\":\"${ZEPHYR_BASE}\", \"ncs_toolchain_preferred\":\"${NCS_TOOLCHAIN_MINIMUM_REQUIRED}\",\"ncs_toolchain_comp_list\":[")
  foreach(comp_toolchain ${NCS_TOOLCHAIN_COMP_LIST})
    set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}{\"toolchain_version\":\"${comp_toolchain}\"},")
  endforeach()
  string(REGEX REPLACE ",$" "" NCS_INFO_JSON_STR ${NCS_INFO_JSON_STR})
  set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}],")

  foreach(board_type thingy;nordic;other)
    set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}\"${board_type}_boards_list\":[")
	string(TOUPPER ${board_type} BOARD_TYPE_UPPER)
    foreach(board ${${BOARD_TYPE_UPPER}_BOARDS})
      set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}{${board}},")
    endforeach()
    string(REGEX REPLACE ",$" "" NCS_INFO_JSON_STR ${NCS_INFO_JSON_STR})
    set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}],")
  endforeach()
  string(REGEX REPLACE ",$" "" NCS_INFO_JSON_STR ${NCS_INFO_JSON_STR})
  set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}},")
endmacro()

set(TEST_CACHE "Testing the cache" CACHE STRING "test")

set(NO_BOILERPLATE TRUE)
find_package(Zephyr 0.0.0 QUIET HINTS $ENV{ZEPHYR_BASE} ${ZEPHYR_BASE})

# Find each NCS Toolchain.
find_package(NcsToolchain 0.0.0 QUIET)
set(NCS_TOOLCHAIN_VERSIONS ${NcsToolchain_CONSIDERED_VERSIONS})
foreach(version ${NCS_TOOLCHAIN_VERSIONS})
  list(GET NcsToolchain_CONSIDERED_CONFIGS 0 ncs_toolchain_config) 
  list(REMOVE_AT NcsToolchain_CONSIDERED_CONFIGS 0)
  get_filename_component(ncs_toolchain_path ${ncs_toolchain_config} DIRECTORY)
  set(NCS_TOOLCHAIN_${version}_PATH ${ncs_toolchain_path}/../)
endforeach()
if(DEFINED NCS_TOOLCHAIN_VERSIONS)
  list(REMOVE_DUPLICATES NCS_TOOLCHAIN_VERSIONS)
endif()

set(NCS_INFO_JSON_STR "{")
set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}\"ncs_toolchains\": [")
foreach(NCS_TOOLCHAIN_VERSION ${NCS_TOOLCHAIN_VERSIONS})
  clear_package_info()
  find_package(NcsToolchain ${NCS_TOOLCHAIN_VERSION} EXACT PATHS ${NCS_TOOLCHAIN_${NCS_TOOLCHAIN_VERSION}_PATH} NO_DEFAULT_PATH)
  set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}{\"ncs_toolchain_version\":\"${NCS_TOOLCHAIN_VERSION}\", ")
  set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}\"ncs_toolchain_path\":\"${NCS_TOOLCHAIN_PATH}\", ")
  set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}\"ncs_toolchain_cmake\":\"${NCS_TOOLCHAIN_CMAKE}\", ")
  set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}\"ncs_toolchain_ninja\":\"${NCS_TOOLCHAIN_NINJA}\", ")
  set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}\"ncs_toolchain_python\":\"${NCS_TOOLCHAIN_PYTHON}\", ")
  set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}\"ncs_toolchain_gnuarmemb_path\":\"${NCS_GNUARMEMB_TOOLCHAIN_PATH}\"},")
endforeach()
string(REGEX REPLACE ",$" "" NCS_INFO_JSON_STR ${NCS_INFO_JSON_STR})
set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}],")

if(DEFINED Zephyr_CONSIDERED_CONFIGS)
  list(REMOVE_DUPLICATES Zephyr_CONSIDERED_CONFIGS)
endif()
set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}\"ncs_releases\": [")
foreach(zephyr ${Zephyr_CONSIDERED_CONFIGS})
  clear_package_info()
  include(${zephyr})
  find_package(Ncs PATHS ${ZEPHYR_BASE}/../* QUIET NO_DEFAULT_PATH NO_POLICY_SCOPE)
  if(Ncs_DIR)
    foreach(NCS_TOOLCHAIN_VERSION ${NCS_TOOLCHAIN_VERSIONS})
      unset(NcsToolchain_FOUND CACHE)
      unset(NcsToolchain_DIR CACHE)
      find_package(NcsToolchain ${NCS_TOOLCHAIN_MINIMUM_REQUIRED} PATHS ${NCS_TOOLCHAIN_${NCS_TOOLCHAIN_VERSION}_PATH} NO_DEFAULT_PATH QUIET)
      if(${NcsToolchain_FOUND})
        list(APPEND NCS_TOOLCHAIN_COMP_LIST ${NCS_TOOLCHAIN_VERSION})
      endif()
    endforeach()
    # Adding boards:
    foreach(root ${NRF_DIR};${ZEPHYR_BASE})
      file(GLOB BOARD_LIST ${root}/boards/arm/*/*_defconfig)

      foreach(board ${BOARD_LIST})
        file(STRINGS ${board} BOARD_SOC REGEX "^[ ]*CONFIG_SOC_SERIES_NRF")
        if(BOARD_SOC)
          get_filename_component(BOARD_NAME ${board} NAME)
          get_filename_component(BOARD_PATH ${board} DIRECTORY)
          string(REGEX REPLACE "_defconfig$" "" BOARD_NAME ${BOARD_NAME})

          if(${BOARD_NAME} MATCHES ^thingy)
            list(APPEND THINGY_BOARDS "\"name\":\"${BOARD_NAME}\", \"path\":\"${BOARD_PATH}\"")
          elseif(${BOARD_NAME} MATCHES ^nrf[^_]*_nrf OR ${root} STREQUAL ${NRF_DIR})
            list(APPEND NORDIC_BOARDS "\"name\":\"${BOARD_NAME}\", \"path\":\"${BOARD_PATH}\"")
          else()
            list(APPEND OTHER_BOARDS "\"name\":\"${BOARD_NAME}\", \"path\":\"${BOARD_PATH}\"")
          endif()
        endif()
      endforeach()
    endforeach()

    append_ncs_package_info()
  endif()
endforeach()
string(REGEX REPLACE ",$" "" NCS_INFO_JSON_STR ${NCS_INFO_JSON_STR})
set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}]")
set(NCS_INFO_JSON_STR "${NCS_INFO_JSON_STR}}")

#message("${NORDIC_BOARDS}")
message("${NCS_INFO_JSON_STR}")
