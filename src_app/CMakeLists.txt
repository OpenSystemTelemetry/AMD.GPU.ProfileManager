# Copyright 2025, Open System Telemetry contributors
# SPDX-License-Identifier: MIT

if(WIN32)
  add_executable(ost_amd_gpu_profilemanager_gui WIN32)
else()
  add_executable(ost_amd_gpu_profilemanager_gui)
endif()

# 
# Sources
#

target_sources(ost_amd_gpu_profilemanager_gui PRIVATE
  "main.cpp"
  "whitelist_preset_manager.cpp"
  "window_details.cpp"
  "window_main.cpp"
)

function(generate_whitelist_sources)
    file(GLOB json_files "${PROJECT_SOURCE_DIR}/data/*.json")

    set(generated_sources "")
    set(registration_calls "")
    set(registration_includes "")

    foreach(json_file ${json_files})
        get_filename_component(preset_name ${json_file} NAME_WE)

        file(READ ${json_file} json_content)
        string(JSON preset_title GET ${json_content} title)

        set(output_header "${CMAKE_CURRENT_BINARY_DIR}/whitelist_preset_${preset_name}.h")
        set(output_source "${CMAKE_CURRENT_BINARY_DIR}/whitelist_preset_${preset_name}.cpp")

        # Generate Header
        file(WRITE ${output_header} "#pragma once\n")
        file(APPEND ${output_header} "#include \"whitelist_preset_item.h\"\n")
        file(APPEND ${output_header} "#include <vector>\n\n")
        file(APPEND ${output_header} "namespace OST::AMD::GPU::ProfileManager {\n")
        file(APPEND ${output_header} "    extern std::vector<WhitelistPresetItem> whitelist_preset_${preset_name};\n")
        file(APPEND ${output_header} "}\n")

        # Generate Source
        string(JSON applications GET ${json_content} applications)

        file(WRITE ${output_source} "#include \"whitelist_preset_${preset_name}.h\"\n\n")
        file(APPEND ${output_source} "namespace OST::AMD::GPU::ProfileManager {\n")
        file(APPEND ${output_source} "    std::vector<WhitelistPresetItem> whitelist_preset_${preset_name} = {\n")

        string(JSON app_count LENGTH ${applications})
        if(app_count GREATER 0)
            math(EXPR last_app_index "${app_count} - 1")
            foreach(i RANGE ${last_app_index})
                string(JSON app GET ${applications} ${i})
                string(JSON app_title GET ${app} title)
                string(JSON app_exe GET ${app} exe)
                file(APPEND ${output_source} "        {L\"${app_title}\", L\"${app_exe}\"},\n")
            endforeach()
        endif()

        file(APPEND ${output_source} "    };\n")
        file(APPEND ${output_source} "}\n")

        list(APPEND generated_sources ${output_header} ${output_source})
        set(registration_includes "${registration_includes}#include \"whitelist_preset_${preset_name}.h\"\n")
        set(registration_calls "${registration_calls}            WhitelistPresetManager::Register(L\"${preset_title}\", whitelist_preset_${preset_name});\n")
    endforeach()

    set(output_registration_source "${CMAKE_CURRENT_BINARY_DIR}/whitelist_preset_registration.cpp")
    file(WRITE ${output_registration_source} "#include \"whitelist_preset_manager.h\"\n")
    file(APPEND ${output_registration_source} "${registration_includes}\n")
    file(APPEND ${output_registration_source} "namespace OST::AMD::GPU::ProfileManager {\n")
    file(APPEND ${output_registration_source} "    namespace WhitelistPresetManager{\n")
    file(APPEND ${output_registration_source} "        void RegisterAll() {\n")
    file(APPEND ${output_registration_source} "${registration_calls}")
    file(APPEND ${output_registration_source} "        }\n")
    file(APPEND ${output_registration_source} "    }\n")
    file(APPEND ${output_registration_source} "}\n")

    list(APPEND generated_sources ${output_registration_source})

    target_sources(ost_amd_gpu_profilemanager_gui PRIVATE ${generated_sources})
    target_include_directories(ost_amd_gpu_profilemanager_gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
endfunction()
generate_whitelist_sources()

#
# Includes
#

target_include_directories(ost_amd_gpu_profilemanager_gui PRIVATE .)



#
# dependencies
#

target_link_libraries(ost_amd_gpu_profilemanager_gui PRIVATE uni.gui)
target_link_libraries(ost_amd_gpu_profilemanager_gui PRIVATE ost_amd_gpu_profilemanager)



#
# Installation
#

install(
  TARGETS ost_amd_gpu_profilemanager_gui
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

if(WIN32)
  if(MSVC)
    install(
      FILES "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:$<TARGET_PDB_FILE:ost_amd_gpu_profilemanager_gui>>"
      DESTINATION "bin"
    )
  endif()

  install(
    FILES "$<TARGET_RUNTIME_DLLS:ost_amd_gpu_profilemanager_gui>"
    DESTINATION "bin"
  )

  add_custom_command(TARGET ost_amd_gpu_profilemanager_gui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t "$<TARGET_FILE_DIR:ost_amd_gpu_profilemanager_gui>" "$<TARGET_RUNTIME_DLLS:ost_amd_gpu_profilemanager_gui>"
    COMMAND_EXPAND_LISTS
  )
endif()
