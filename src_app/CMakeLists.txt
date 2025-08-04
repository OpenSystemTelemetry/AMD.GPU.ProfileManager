# Copyright 2025, Open System Telemetry contributors
# SPDX-License-Identifier: MIT

if(WIN32)
  add_executable(ost_amd_gpu_profilemanager_gui WIN32)
else()
  add_executable(ost_amd_gpu_profilemanager_gui
          window_details.cpp
          app_info.h)
endif()

target_sources(ost_amd_gpu_profilemanager_gui PRIVATE
  "main.cpp"
  "window_details.cpp"
  "window_main.cpp"
)

# dependencies
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
