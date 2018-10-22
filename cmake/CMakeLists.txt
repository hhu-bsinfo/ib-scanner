# Copyright (C) 2018 Heinrich-Heine-Universitaet Duesseldorf,
# Institute of Computer Science, Department Operating Systems
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>

cmake_minimum_required(VERSION 3.5)
include(ExternalProject)

# Set version
execute_process(COMMAND git describe --tags --abbrev=0 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE IBPERFMON_VERSION ERROR_QUIET)
string(STRIP "${IBPERFMON_VERSION}" IBPERFMON_VERSION)
if (IBPERFMON_VERSION STREQUAL "")
    set(IBPERFMON_VERSION "v0.0.0")
endif ()

# Set git revision
execute_process(COMMAND git log -1 --format=%h --date=short HEAD WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE IBPERFMON_GIT_REV ERROR_QUIET)
string(STRIP "${IBPERFMON_GIT_REV}" IBPERFMON_GIT_REV)
if (IBPERFMON_GIT_REV STREQUAL "")
    set(IBPERFMON_GIT_REV "not available")
endif ()

# Set git branch
execute_process(COMMAND git rev-parse --symbolic-full-name --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE IBPERFMON_GIT_BRANCH ERROR_QUIET)
string(STRIP "${IBPERFMON_GIT_BRANCH}" IBPERFMON_GIT_BRANCH)
if (IBPERFMON_GIT_BRANCH STREQUAL "")
    set(IBPERFMON_GIT_BRANCH "not available")
endif ()

# Set build date
string(TIMESTAMP IBPERFMON_BUILD_DATE "%Y-%m-%d %H:%M:%S")

# Print build information

message("IbPerfMon ${IBPERFMON_VERSION} - git ${IBPERFMON_GIT_REV}(${IBPERFMON_GIT_BRANCH}), \
build date: ${IBPERFMON_BUILD_DATE}")

# Set preprocessor defines
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIBPERFMON_VERSION='${IBPERFMON_VERSION}'\
    -DIBPERFMON_GIT_REV='${IBPERFMON_GIT_REV}' -DIBPERFMON_GIT_BRANCH='${IBPERFMON_GIT_BRANCH}' \
    -DIBPERFMON_BUILD_DATE='${IBPERFMON_BUILD_DATE}'")

# Set source directory variables
set(IBPERFMON_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
set(IBPERFMON_SRC_DIR "${IBPERFMON_ROOT_DIR}/src")

# Set output directory variables
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ar)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Force out of source build
message(STATUS "Force out of source build check...")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource)
if (insource)
    message(FATAL_ERROR "Please do not build in your source dir. Use a dedicated build folder!")
endif ()

# Set compiler-flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# For development: Uncomment this line to compile with sanitizers to debug memory errors and leaks
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-omit-frame-pointer -fsanitize=address")

# Add IbPerfLib as external project
ExternalProject_Add(IbPerfLib_git
        GIT_REPOSITORY https://github.com/hhu-bsinfo/ibperf-lib.git
        GIT_TAG ${IBPERFMON_GIT_BRANCH}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/IbPerfLib
        INSTALL_COMMAND "")

ExternalProject_Get_Property(IbPerfLib_git binary_dir)
ExternalProject_Get_Property(IbPerfLib_git source_dir)

add_library(IbPerfLib STATIC IMPORTED ../src/IbPerfMon/BuildConfig.cpp ../src/IbPerfMon/BuildConfig.h)
set_property(TARGET IbPerfLib PROPERTY IMPORTED_LOCATION ${binary_dir}/ar/libIbPerfLib.a)

add_dependencies(IbPerfLib IbPerfLib_git)

include_directories(${source_dir}/src)

# Add subdirectories
add_subdirectory(CursesLib)
add_subdirectory(WindowTest)
add_subdirectory(IbPerfMon)
