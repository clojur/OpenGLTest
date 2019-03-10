add_subdirectory(MFCGUI)
add_subdirectory(MyOpenGL)
add_subdirectory(OrkRender)


set(CopyRes ${CMAKE_CURRENT_SOURCE_DIR}/MyOpenGL/Resource)
set(GlewLib ${CMAKE_CURRENT_SOURCE_DIR}/SDKs/glew/bin/${CMAKE_BUILD_TYPE})
set(NoesisLib ${CMAKE_CURRENT_SOURCE_DIR}/SDKs/NoesisGUI/Bin/windows_x86)

set(OutDir ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE})

add_custom_command(OUTPUT  COPY_RES
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CopyRes}  ${OutDir}/Resource
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${GlewLib} ${OutDir}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${NoesisLib} ${OutDir}
 )

add_custom_target(CopyTask ALL DEPENDS COPY_RES)