# Copyright 2025, Open System Telemetry contributors
# SPDX-License-Identifier: MIT

add_library(amd_adl_sdk STATIC)

target_sources(amd_adl_sdk PRIVATE src/amd_adl_sdk.c)

target_include_directories(amd_adl_sdk PUBLIC include)
target_include_directories(amd_adl_sdk PUBLIC ../amd-display-library/include)

