# Copyright 2025, Open System Telemetry contributors
# SPDX-License-Identifier: MIT

add_library(ost_amd_gpu_profilemanager STATIC)

target_sources(ost_amd_gpu_profilemanager PRIVATE
    src/amd_gpu_profilemanager_application.cpp
    src/amd_gpu_profilemanager_application_combined.cpp
    src/amd_gpu_profilemanager_area.cpp
    src/amd_gpu_profilemanager_db.cpp
    src/amd_gpu_profilemanager_profile.cpp
    src/amd_gpu_profilemanager_property.cpp
    src/amd_gpu_profilemanager_use.cpp
    src/amd_gpu_profilemanager_utils.cpp    
)

target_include_directories(ost_amd_gpu_profilemanager PUBLIC include)

target_link_libraries(ost_amd_gpu_profilemanager PUBLIC amd_adl_sdk)
target_link_libraries(ost_amd_gpu_profilemanager PUBLIC nlohmann_json)
target_link_libraries(ost_amd_gpu_profilemanager PUBLIC magic_enum)
