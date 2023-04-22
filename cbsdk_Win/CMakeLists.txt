cmake_minimum_required(VERSION 3.12)

project(BlackrockTimestampsLSL
	DESCRIPTION "Capture Blackrock Neurotech timestamps and re-stream over an LSL outlet"
	HOMEPAGE_URL "https://github.com/labstreaminglayer/App-BlackrockTimestamps/"
	LANGUAGES C CXX
	VERSION 1.0)

# Needed for customized MacOSXBundleInfo.plist.in
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

# Dependencies
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(TARGET_PLATFORM amd64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(TARGET_PLATFORM i386)
endif()
include(FetchContent)

## Prefetch
FetchContent_Populate(
		lsl_pkg
		URL        "https://github.com/sccn/liblsl/releases/download/v1.16.1/liblsl-1.16.1-Win_${TARGET_PLATFORM}.zip"
		SOURCE_DIR liblsl
)
FetchContent_Declare(
		cxxopts
		GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
		GIT_TAG        v3.1.1
		GIT_SHALLOW    TRUE
)
set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CXXOPTS_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(CXXOPTS_ENABLE_WARNINGS OFF CACHE BOOL "" FORCE)

## Threads
find_package(Threads REQUIRED)

## LSL
find_package(LSL REQUIRED
    HINTS
        ${LSL_INSTALL_ROOT}
        "${CMAKE_CURRENT_BINARY_DIR}/liblsl"
    PATH_SUFFIXES
        lib
        share/LSL
)

## cxxopts
FetchContent_MakeAvailable(cxxopts)

## cbsdk
find_package(CBSDK REQUIRED)
get_target_property(cbsdk_proto_ver CBSDK::cbsdk VERSION)
string(SUBSTRING ${cbsdk_proto_ver} 0 1 cbsdk_proto_ver_major)

# Targets
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    src/main.cpp
)

if(${cbsdk_proto_ver_major} GREATER 3)
	# Protocol 4 introduced cbPKT_HEADER and 64-bit timestamps
	target_compile_definitions(${PROJECT_NAME} PUBLIC CBSDK_PROTO_4)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Threads::Threads
    LSL::lsl
	cxxopts
	CBSDK::cbsdk
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
)

include(GNUInstallDirs) # definitions of CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_INCLUDEDIR and others
install(TARGETS ${PROJECT_NAME}
        # these get default values from GNUInstallDirs, no need to set them
        BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # bin
)
install(
    FILES
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
    TYPE BIN
)
