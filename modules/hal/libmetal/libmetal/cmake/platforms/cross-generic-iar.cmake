set (CMAKE_SYSTEM_NAME "Generic"             CACHE STRING "")
set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER CACHE STRING "")
set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER CACHE STRING "")
set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER CACHE STRING "")

include (CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER   ("icc${CROSS_SUFFIX}" IAR)
CMAKE_FORCE_CXX_COMPILER ("icc${CROSS_SUFFIX} --eec++" IAR)

# vim: expandtab:ts=2:sw=2:smartindent
