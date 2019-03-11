set(LibName MFCGUI)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SDKDir ../SDks)

include_directories(${SDKDir})

include_directories(${SDKDir}\\opengl\\include)
link_directories(${SDKDir}\\opengl\\lib\\${CMAKE_BUILD_TYPE})

include_directories(${SDKDir}\\glew\\include)
link_directories(${SDKDir}\\glew\\lib\\${CMAKE_BUILD_TYPE})

link_directories(${SDKDir}\\soil\\lib\\${CMAKE_BUILD_TYPE})

include_directories("${SDKDir}\\NoesisGUI\\include\\Packages\\Render\\GLRenderDevice\\Include")
include_directories("${SDKDir}\\NoesisGUI\\include\\Packages\\App\\Launcher\\Include")
include_directories("${SDKDir}\\NoesisGUI\\include")
link_directories("${SDKDir}\\NoesisGUI\\Lib\\windows_x86")

include_directories("D:/Microsoft\ Visual\ Studio\\2017\\Professional\\VC\\Tools\\MSVC\\14.13.26128\\atlmfc\\include")

set(Common_Heads
    stdafx.h
    resource.h 
    targetver.h   
)

set(Frame_Files 
    MainFrm.h 
    MainFrm.cpp
)

set(WinAPP_Files 
    OGLGUI.h 
    OGLGUI.cpp
)

set(DOC_Files 
    OGLGUIDoc.h
    OGLGUIDoc.cpp
)

set(View_Files 
    OGLGUIView.h
    OGLGUIView.cpp
    CalendarBar.h
    CalendarBar.cpp
)

file(GLOB CodeFiles res/*)
set(ResourceFiles
	OGLGUI.rc
	${CodeFiles}
)

source_group(Common_Heads FILES ${Common_Heads})
source_group(WinAPP_Files FILES ${WinAPP_Files})
source_group(Frame_Files FILES ${Frame_Files})
source_group(DOC_Files FILES ${DOC_Files})
source_group(View_Files FILES ${View_Files})
source_group(ResourceFiles FILES ${ResourceFiles})

if(WIN32)

add_definitions(-DWIN32_LEAN_AND_MEAN -DUNICODE -D_UNICODE)
SET(CMAKE_MFC_FLAG 2)
ADD_DEFINITIONS(-D_AFXDLL)

endif()

if (USE_NoesisGUI_Test)
	add_definitions(-DUSE_NOESISGUI_TEST)
endif()


#Debug������Ŀ�����������׺d
if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

#�����ⲿ��
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(ExternLib  MyOpenGLd)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(ExternLib MyOpenGL)
else()
	set(ExternLib MyOpenGL)
endif()


add_executable(${LibName}  WIN32 
    ${Common_Heads}
    ${WinAPP_Files}
    ${Frame_Files}
    ${DOC_Files}
    ${View_Files}
	${ResourceFiles}
)

#IF(WIN32)
#   SET(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS") 
#ENDIF()

#������������/SUBSYSTEM:WINDOWS
#if(WIN32)
#	set_target_properties(${LibName}  
#		PROPERTIES
#		LINK_FLAGS /SUBSYSTEM:WINDOWS
#	)
#endif()

target_link_libraries(${LibName} ${ExternLib})