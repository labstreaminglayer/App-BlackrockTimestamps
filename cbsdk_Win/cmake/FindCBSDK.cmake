# FindCBSDK
# -------------
#
# Find Cerebus SDK CBSDK
#
# Use this module by invoking find_package with the form::
#
#    find_package( CBSDK [REQUIRED] )
#
#
#    target_link_libraries( ${PROJECT_NAME} CBSDK::cbsdk )
#
# =============================================================================
#
# Some portions of this script are copied from other similar CMake find package scripts.
# Copyright (c) 2023 Chadwick Boulay
# Distributed under the MIT License.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# =============================================================================


if(NOT DEFINED CBSDK_ROOT)
    set(CBSDK_ROOT "C:\\Program Files (x86)\\Blackrock Microsystems\\NeuroPort Windows Suite\\cbsdk")
endif(NOT DEFINED CBSDK_ROOT)
set(CBSDK_ROOT "${CBSDK_ROOT}" CACHE PATH "CBSDK root directory to look in.")

# Target Platform
set(TARGET_PLATFORM)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(TARGET_PLATFORM x64)
endif()

find_library(CBSDK_LIBRARY
    NAMES
        cbsdk${TARGET_PLATFORM}
    PATHS
        "${CBSDK_ROOT}"
        "C:\\Program Files (x86)\\Blackrock Microsystems\\Cerebus Central Suite\\cbsdk"
        "C:\\Program Files\\Blackrock Microsystems\\NeuroPort Windows Suite\\cbsdk"
        "C:\\Program Files\\Blackrock Microsystems\\Cerebus Central Suite\\cbsdk"
    PATH_SUFFIXES
        lib
)

find_path(CBSDK_INCLUDE_DIR
    NAMES
        cbsdk.h
    PATHS
        "${CBSDK_ROOT}"
    PATH_SUFFIXES
        include
)

# handle the QUIETLY and REQUIRED arguments and set xxx_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CBSDK
    DEFAULT_MSG
    CBSDK_LIBRARY
    CBSDK_INCLUDE_DIR
)

if(CBSDK_FOUND)
    set(CBSDK_LIBRARIES cbsdk${TARGET_PLATFORM})
    set(CBSDK_INCLUDE_DIRS ${CBSDK_INCLUDE_DIR})

    # Get version from cbhwlib.h cbVERSION_MAJOR.cbVERSION_MINOR
    #  Technically, this is the protocl version, not the library version.
    file(READ "${CBSDK_INCLUDE_DIR}/cbhwlib.h" cbhw)
    string(REGEX MATCH "cbVERSION_MAJOR +([0-9]+)" proto_ver_major ${cbhw})
    set(proto_ver_major ${CMAKE_MATCH_1})
    string(REGEX MATCH "cbVERSION_MINOR +([0-9]+)" _ ${cbhw})
    set(proto_ver_minor ${CMAKE_MATCH_1})

    add_library(CBSDK::cbsdk SHARED IMPORTED)
    set_target_properties(CBSDK::cbsdk
        PROPERTIES
            IMPORTED_LOCATION ${CBSDK_ROOT}/lib/cbsdk${TARGET_PLATFORM}.dll  # The dll
            INTERFACE_INCLUDE_DIRECTORIES ${CBSDK_INCLUDE_DIRS}
            IMPORTED_IMPLIB ${CBSDK_ROOT}/lib/cbsdk${TARGET_PLATFORM}.lib
            VERSION ${proto_ver_major}.${proto_ver_minor}
    )


    
endif(CBSDK_FOUND)

mark_as_advanced(
    CBSDK_LIBRARY
    CBSDK_INCLUDE_DIR
)

if(CBSDK_FOUND)
    mark_as_advanced(CBSDK_ROOT)
endif()
