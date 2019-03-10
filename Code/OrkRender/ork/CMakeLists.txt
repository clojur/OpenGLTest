set(LibName ork)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
macro(MakeGroup subDir)
	file(GLOB CodeFiles ${subDir}/*.h ${subDir}/*.cpp)
	set(${subDir}Files
			${CodeFiles}
	)
	source_group(${subDir} FILES ${${subDir}Files})
endmacro()

MakeGroup(core)
MakeGroup(math)
MakeGroup(render)
MakeGroup(resource)
MakeGroup(scenegraph)
MakeGroup(taskgraph)
MakeGroup(ui)
MakeGroup(util)
MakeGroup(stbi)
MakeGroup(tinyxml)


if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(ExternLib  opengl glu freeglutd glew32d glfw3  pthreadVSE2)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(ExternLib opengl glu freeglut glew32 glfw3  pthreadVSE2)
else()
	set(ExternLib opengl glu freeglut glew32 glfw3  pthreadVSE2)
endif()

#Debug配置下目标输出名带后缀d
if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

add_library(${LibName} SHARED
 ${coreFiles}
 ${mathFiles}
 ${renderFiles}
 ${resourceFiles}
 ${scenegraphFiles}
 ${taskgraphFiles}
 ${uiFiles}
 ${utilFiles}
 ${stbiFiles}
 ${tinyxmlFiles}
)

#添加链接命令/SAFESEH:NO
if(WIN32)
	set_target_properties(${LibName}  
		PROPERTIES
		LINK_FLAGS /SAFESEH:NO
	)
endif()

target_link_libraries(${LibName}  ${ExternLib})
