set(LibName MyOpenGL)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SDKDir "${CMAKE_CURRENT_SOURCE_DIR}/../SDks")


include_directories(${SDKDir})

include_directories(${SDKDir}\\opengl\\include)
link_directories(${SDKDir}\\opengl\\lib)

include_directories(${SDKDir}\\glew\\include)
link_directories(${SDKDir}\\glew\\lib)

link_directories(${SDKDir}\\soil\\lib)

include_directories("${SDKDir}\\NoesisGUI\\include\\Packages\\Render\\GLRenderDevice\\Include")
include_directories("${SDKDir}\\NoesisGUI\\include\\Packages\\App\\Launcher\\Include")
include_directories("${SDKDir}\\NoesisGUI\\include")
link_directories("${SDKDir}\\NoesisGUI\\Lib\\windows_x86")



add_definitions(-DMyOpenGL_DLL -DWIN32_LEAN_AND_MEAN -DUNICODE)

if(USE_NoesisGUI_Test)
	add_definitions(-DUSE_NOESISGUI_TEST)
endif()

SET(CMAKE_MFC_FLAG 2)#0表示使用标准的MFC，1表示静态MFC,2表示共享
ADD_DEFINITIONS(-D_AFXDLL)#如果使用动态库的MFC，则加上_AFXDLL

set(Common
common.h 
macro.h
targetver.h
)

set(OpenGL
OpenGL.h
OpenGL.cpp
OpenGLObject.h
OpenGLObject.cpp
Shader.h
Shader.cpp
)

set(Math
Vec3.h
Vec3.cpp
)

set(res
Resource.h
)

file(GLOB NoesisRes Resource/Noesis/*.xaml Resource/Noesis/*.ttf)
set(Noesis
	${NoesisRes}
)

file(GLOB ShaderRes Resource/ShaderFile/*.glsl)
set(Shader
	${ShaderRes}
)

file(GLOB TextureRes Resource/Texture/*.png Resource/Texture/*.jpg)
set(Texture
	${TextureRes}
)

source_group(Common FILES ${Common})
source_group(OpenGL FILES ${OpenGL})
source_group(Math FILES ${Math})
source_group(res FILES ${res})
source_group(res\\NoesisRes FILES ${Noesis})
source_group(res\\ShaderRes FILES ${Shader})
source_group(res\\TextureRes FILES ${Texture})


if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(ExternLib  opengl32 glew32d SOIL Noesis NoesisApp)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(ExternLib opengl glew32 SOIL  Noesis NoesisApp)
else()
	set(ExternLib opengl glew32 SOIL  Noesis NoesisApp)
endif()


# set(Bin2hTool G:\\NoesisGUI\\NoesisSDK\\Src\\Tools\\Bin2h\\bin2h.exe)

# add_custom_target(GenertingHead ALL DEPENDS ${Bin2hTool})

#  add_custom_command(TARGET GenertingHead
#     PRE_BUILD
#     COMMAND mkdir GenertingHead
#     COMMAND ${Bin2hTool}  "${CMAKE_CURRENT_SOURCE_DIR}/Settings.xaml" > "${CMAKE_CURRENT_SOURCE_DIR}/GenertingHead/Settings.xaml.bin.h"
#     COMMAND ${Bin2hTool}  "${CMAKE_CURRENT_SOURCE_DIR}/HermeneusOne-Regular.ttf" > "${CMAKE_CURRENT_SOURCE_DIR}/GenertingHead/HermeneusOne-Regular.ttf.bin.h"
#     COMMENT "Noesis res Head Generating oK！"
#  )

#Debug配置下添加后缀d
if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

add_library(${LibName} SHARED
${Common}
${OpenGL}
${Math}
${res}
${Noesis}
${Shader} 
${Texture}
)

target_link_libraries(${LibName} ${ExternLib})

#添加链接命令/SAFESEH:NO
if(WIN32)
	set_target_properties(${LibName}  
		PROPERTIES
		LINK_FLAGS /SAFESEH:NO
	)
endif()