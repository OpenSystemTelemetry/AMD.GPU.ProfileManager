# Copyright 2025, Open System Telemetry contributors
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 4.0)

project(amd_gpu_profile_manager)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 26)

add_subdirectory(3rdparty/amd-display-library-extra)

include(3rdparty/cpm.cmake)
include(3rdparty/cpm-nlohmann_json.cmake)
include(3rdparty/cpm-magic_enum.cmake)
include(3rdparty/cpm-uni.gui.cmake)

add_subdirectory(src)
add_subdirectory(src_app)
