#!/usr/bin/env bash

#
#    Copyright (c) 2020 Project CHIP Authors
#    Copyright (c) 2019 Google LLC.
#    Copyright (c) 2014-2017 Nest Labs, Inc.
#    All rights reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

#
#    Description:
#      This file implements a script to parse a version number from
#      the command line, split it into its constituent components, and
#      then auto-generate a C-language header file containing
#      preprocessor mnemonics for those components.
#

#
# usage
#
# Description:
#   This routine prints out the proper command line usage for this
#   program
#
# Input(s):
#   status - Flag determining what usage information will be printed and what
#            the exit status of the program will be after the information is
#            printed.
#
# Output(s):
#   N/A
#
# Returns:
#   This subroutine does not return.
#
usage() {
    declare status="$1"
    shift

    ((status)) && echo "Error: $me: $*
Try '$me --help' for more information." >&2

    echo "Usage: $me [ options ... ] [ -o OUTPUT ] <version>" >&2

    ((!status)) && echo "General Options:

  -h, --help                 Display this information.

Input and Output Options:

  -o, --output PATH          Specify PATH as the output file to write with the
                             generated ouput (default: standard output).
"
    exit "$status"
}

me=${0##*/}

declare outfile=

# Parse options from the command line
while true; do
    case $1 in
        --help | -h)
            usage 0
            ;;
        --output | -o)
            shift
            outfile=$1
            shift
            ;;
        -*)
            usage 1 "unknown option \"$1\""
            ;;
        *)
            break
            ;;
    esac
done

# At this point, the version to parse should be the only argument.
((${#@} < 1)) && usage 2 please pass a version on the command line
((${#@} > 1)) && usage 2 too many arguments

declare chip_version="$1"

# Establish the copyright year and script base name scalar variables
# for substitution in the here document output.

declare first_year
declare current_year
declare copyright_year
first_year=2020
current_year="$(date "+%Y")"
copyright_year="$first_year"

((first_year < current_year)) && {
    copyright_year+="-$current_year"
}

declare chip_major
declare chip_minor
declare chip_patch
declare chip_extra

# Parse the version into its constituent components.
read -r chip_major chip_minor chip_patch chip_extra <<<"${chip_version//./ }"

# preserve the "." in any extra
[[ -n ${chip_extra} ]] && chip_extra=".$chip_extra"

# Major and minor should be numbers. If they're not use the full input version as
#  the "extra" and set everything else back to 0
[ "$chip_major" -eq "$chip_major" -a "$chip_minor" -eq "$chip_minor" ] 2>/dev/null || {
    chip_major=0
    chip_minor=0
    chip_patch=0
    chip_extra=.$chip_version
}

# If chip_patch isn't a number, set chip_patch to zero and push the non-number back into chip_extra
[ "$chip_patch" -eq "$chip_patch" ] || {
    chip_extra=.$chip_patch$chip_extra
    chip_patch=0
}

# Canonically recompose the version from its components.  zero-patchlevel is no patchlevel
chip_version=$chip_major.$chip_minor

# Zero patchlevel means no patchlevel
((chip_patch)) && chip_version+=.$chip_patch

chip_version+=$chip_extra

# Initialize the output stream, if is a filename
[[ -n $outfile ]] && exec >"$outfile"

# Generate the output file as a "here document" with
# variable interpolation (escaping where necessary).
cat <<EOF
/*
 *
 *    Copyright (c) ${copyright_year} Project CHIP Authors
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

/**
 *    @file
 *      This file defines constants and macros for introspecting and
 *      manipulating CHIP versions.
 *
 *      !!! WARNING !!! WARNING !!! WARNING !!! WARNING !!!
 *
 *      DO NOT EDIT THIS FILE! This file is automatically-generated by
 *      the '${me}' script.
 *
 *      The constants and macros defined here may be used to, for ,
 *      example, conditionally-compile older, newer, or changed CHIP
 *      APIs based on the CHIP version. For example:
 *
 *          @code
 *          #if CHIP_VERSION_CODE >= CHIP_VERSION_CODE_ENCODE(1, 5, 0)
 *              ...
 *          #else
 *              ...
 *          #endif
 *          @endcode
 *
 */

#ifndef CHIP_VERSION_H_
#define CHIP_VERSION_H_

#define _CHIP_VERSION_CODE_MAJOR_WIDTH     8
#define _CHIP_VERSION_CODE_MINOR_WIDTH     8
#define _CHIP_VERSION_CODE_PATCH_WIDTH     8

#define _CHIP_VERSION_CODE_MAJOR_MASK     ((1 << _CHIP_VERSION_CODE_MAJOR_WIDTH)  - 1)
#define _CHIP_VERSION_CODE_MINOR_MASK     ((1 << _CHIP_VERSION_CODE_MINOR_WIDTH)  - 1)
#define _CHIP_VERSION_CODE_PATCH_MASK     ((1 << _CHIP_VERSION_CODE_PATCH_WIDTH)  - 1)

#define _CHIP_VERSION_CODE_MAJOR_SHIFT    24
#define _CHIP_VERSION_CODE_MINOR_SHIFT    16
#define _CHIP_VERSION_CODE_PATCH_SHIFT     8

/**
 *  @def CHIP_VERSION_CODE_ENCODE(major, minor, patch)
 *
 *  @brief
 *    Encode a CHIP version code from its constituent @a major, @a minor, and @a patch
 *    components.
 *
 *    This macro may be used in conjunction with CHIP_VERSION_CODE to, for
 *    example, conditionally-compile older, newer, or changed CHIP APIs based
 *    on the CHIP version. For example:
 *
 *        @code
 *        #if CHIP_VERSION_CODE >= CHIP_VERSION_CODE_ENCODE(1, 5, 0)
 *            ...
 *        #else
 *            ...
 *        #endif
 *        @endcode
 *
 */
#define CHIP_VERSION_CODE_ENCODE(major, minor, patch)                                    \\
    ((((major)  & _CHIP_VERSION_CODE_MAJOR_MASK)  << _CHIP_VERSION_CODE_MAJOR_SHIFT)  | \\
     (((minor)  & _CHIP_VERSION_CODE_MINOR_MASK)  << _CHIP_VERSION_CODE_MINOR_SHIFT)  | \\
     (((patch)  & _CHIP_VERSION_CODE_PATCH_MASK)  << _CHIP_VERSION_CODE_PATCH_SHIFT))

/**
 *  @def CHIP_VERSION_CODE_DECODE_MAJOR(code)
 *
 *  @brief
 *    Decode a CHIP major version component from a CHIP version @a code.
 *
 */
#define CHIP_VERSION_CODE_DECODE_MAJOR(code)  (((code) >> _CHIP_VERSION_CODE_MAJOR_SHIFT)  & _CHIP_VERSION_CODE_MAJOR_MASK)

/**
 *  @def CHIP_VERSION_CODE_DECODE_MINOR(code)
 *
 *  @brief
 *    Decode a CHIP minor version component from a CHIP version @a code.
 *
 */
#define CHIP_VERSION_CODE_DECODE_MINOR(code)  (((code) >> _CHIP_VERSION_CODE_MINOR_SHIFT)  & _CHIP_VERSION_CODE_MINOR_MASK)

/**
 *  @def CHIP_VERSION_CODE_DECODE_PATCH(code)
 *
 *  @brief
 *    Decode a CHIP patch version component from a CHIP version @a code.
 *
 */
#define CHIP_VERSION_CODE_DECODE_PATCH(code)  (((code) >> _CHIP_VERSION_CODE_PATCH_SHIFT)  & _CHIP_VERSION_CODE_PATCH_MASK)

/**
 *  @def CHIP_VERSION_MAJOR
 *
 *  @brief
 *    The CHIP version major component, as an unsigned integer.
 *
 */
#define CHIP_VERSION_MAJOR               ${chip_major}

/**
 *  @def CHIP_VERSION_MINOR
 *
 *  @brief
 *    The CHIP version minor component, as an unsigned integer.
 *
 */
#define CHIP_VERSION_MINOR               ${chip_minor}

/**
 *  @def CHIP_VERSION_PATCH
 *
 *  @brief
 *    The CHIP version patch component, as an unsigned integer.
 *
 */
#define CHIP_VERSION_PATCH               ${chip_patch}

/**
 *  @def CHIP_VERSION_EXTRA
 *
 *  @brief
 *    The CHIP version extra component, as a quoted C string.
 *
 */
#define CHIP_VERSION_EXTRA               "${chip_extra}"

/**
 *  @def CHIP_VERSION_STRING
 *
 *  @brief
 *    The CHIP version, as a quoted C string.
 *
 */
#define CHIP_VERSION_STRING              "${chip_version}"

/**
 *  @def CHIP_VERSION_CODE
 *
 *  @brief
 *    The CHIP version, including the major, minor, and patch components,
 *    encoded as an unsigned integer.
 *
 *    This macro may be used in conjunction with CHIP_VERSION_CODE_ENCODE
 *    to, for example, conditionally-compile older, newer, or changed CHIP
 *    APIs based on the CHIP version. For example:
 *
 *        @code
 *        #if CHIP_VERSION_CODE >= CHIP_VERSION_CODE_ENCODE(1, 5, 0)
 *            ...
 *        #else
 *            ...
 *        #endif
 *        @endcode
 *
 */
#define CHIP_VERSION_CODE                CHIP_VERSION_CODE_ENCODE( \\
          CHIP_VERSION_MAJOR,                                      \\
          CHIP_VERSION_MINOR,                                      \\
          CHIP_VERSION_PATCH                                       \\
        )

#endif /* CHIP_VERSION_H_ */
EOF
