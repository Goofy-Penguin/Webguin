cmake_minimum_required (VERSION 3.8)

include(FetchContent)

# version
set(WEBGUIN_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(WEBGUIN_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(WEBGUIN_VERSION ${WEBGUIN_VERSION_MAJOR}.${WEBGUIN_VERSION_MINOR} CACHE STRING "version" FORCE)

# flags
set(WEBGUIN_DOWNLOAD_MAINFRAME true CACHE BOOL "use mainframe from git repo" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPDLOG_FMT_EXTERNAL")

# msvsc needs this as cmake doesn't provice for it yet
# TODO: once cmake supports msvsc std latest remove this
if (MSVC_VERSION GREATER_EQUAL "1900")
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
	if (_cpp_latest_flag_supported)
		add_compile_options("/std:c++latest")
	endif()
endif()

# libraries
FetchContent_Declare(mainframe GIT_REPOSITORY https://github.com/Goofy-Penguin/Mainframe.git)
FetchContent_MakeAvailable(mainframe)

include("code/cmake/HunterGate.cmake")
HunterGate(
	URL "https://github.com/cpp-pm/hunter/archive/v0.23.254.tar.gz"
	SHA1 "017e688b9848c1f695303e8cfe9f35d40e154112"
)

project ("webguin" VERSION ${WEBGUIN_VERSION})

hunter_add_package(fmt)
hunter_add_package(nlohmann_json)

find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)

if (WIN32)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")
endif()

function(WGAddExecutable projname)
	message("detected exe:" ${CMAKE_SOURCE_DIR}/exec/${output_target})
	message("detected FROM:" ${CMAKE_CURRENT_BINARY_DIR}/${output_target})

	if (WIN32)
		add_custom_command(
			TARGET ${output_target}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/${output_target}.exe ${CMAKE_SOURCE_DIR}/exec/${output_target}.exe)
	else()
		add_custom_command(
			TARGET ${output_target}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${output_target} ${CMAKE_SOURCE_DIR}/exec/${output_target})
	endif()
endfunction()

add_subdirectory("code")
add_subdirectory("examples")
