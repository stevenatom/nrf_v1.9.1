include(Compiler/XLClang)
__compiler_xlclang(C)

set(CMAKE_C_COMPILE_OPTIONS_EXPLICIT_LANGUAGE -x c)

if (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 13.1.1)
  set(CMAKE_C90_STANDARD_COMPILE_OPTION  "-std=c89")
  set(CMAKE_C90_EXTENSION_COMPILE_OPTION "-std=gnu89")
  set(CMAKE_C90_STANDARD__HAS_FULL_SUPPORT ON)
  set(CMAKE_C99_STANDARD_COMPILE_OPTION  "-std=c99")
  set(CMAKE_C99_EXTENSION_COMPILE_OPTION "-std=gnu99")
  set(CMAKE_C99_STANDARD__HAS_FULL_SUPPORT ON)
  set(CMAKE_C11_STANDARD_COMPILE_OPTION  "-qlanglvl=extc1x")
  set(CMAKE_C11_EXTENSION_COMPILE_OPTION "-qlanglvl=extc1x")
  if (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 13.1.2)
    set(CMAKE_C11_STANDARD_COMPILE_OPTION  "-std=c11")
    set(CMAKE_C11_EXTENSION_COMPILE_OPTION "-std=gnu11")
    set(CMAKE_C11_STANDARD__HAS_FULL_SUPPORT ON)
  endif ()
endif()

__compiler_check_default_language_standard(C 13.1.1 99)
