set_property(GLOBAL PROPERTY USE_FOLDERS On)

option(BUILD_TOOLS_ONLY "help string describing option")

cmake_minimum_required(VERSION 3.1)
macro(remove_file arg_list file_remove)
	file(GLOB RemoveItems_Cpp ${file_remove})
	list(REMOVE_ITEM ${arg_list} ${RemoveItems_Cpp})
endmacro(remove_file)

macro(link_NFSDK projName)
if(UNIX)
	add_dependencies(${projName} NFNetPlugin NFCore NFMessageDefine)
	target_link_libraries(${projName} NFCore protobuf NFMessageDefine NFNetPlugin event event_core)
else()
	add_dependencies(${projName} NFNetPlugin NFCore NFMessageDefine)
	target_link_libraries(${projName} NFCore)
endif()
endmacro(link_NFSDK)

macro(add_post_cmd_win strProjectName destDir ext dbgSuffix)
	
endmacro()

macro(add_post_cmd_linux strProjectName destDir ext dbgSuffix dllPrefix)
	
endmacro()

		

project(NoahFrame)

set(SolutionDir ${PROJECT_SOURCE_DIR})
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
		set(NFOutPutDir ${PROJECT_SOURCE_DIR}/_Out/Debug)
	else()
		set(NFOutPutDir ${PROJECT_SOURCE_DIR}/_Out/Release)
	endif()
else()
	set(NFOutPutDir ${PROJECT_SOURCE_DIR}/_Out/)
endif()


# message(STATUS "SolutionDir=" ${SolutionDir})
include_directories(
${SolutionDir} 
${SolutionDir}/Dependencies/ 
${SolutionDir}/Dependencies/Theron/Include/ 
${SolutionDir}/Dependencies/Theron/Include/External/ 
${SolutionDir}/Dependencies/protobuf/ 
${SolutionDir}/Dependencies/protobuf/src/
${SolutionDir}/Dependencies/libevent/
${SolutionDir}/Dependencies/libevent/include/
${SolutionDir}/Dependencies/libevent/compat/
${SolutionDir}/Dependencies/lua/
${SolutionDir}/Dependencies/asio/
${SolutionDir}/Dependencies/websocketpp/
)

#message("Build Type:"${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS})

if(UNIX)
	if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
		set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -Wall -g -ggdb -DDEBUG -fPIC")
	else()
		set(CMAKE_BUILD_TYPE "Release")
		set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fPIC -std=c++11 -O3 -Wall")
	endif()
	set(LDFLAGS "$ENV{LDFLAGS} -stdlib=libc++")
	add_definitions(-std=c++11)
else()
	SET ( CMAKE_CXX_FLAGS "-D_X64 -D_WINDOWS /EHsc" )
	include_directories(${SolutionDir}/Dependencies/libevent/WIN32-Code/)
endif()
if(UNIX)
	if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
		set(DependenciesLibPath ${SolutionDir}/Dependencies/lib ${SolutionDir}/_Out/Debug/)
	else()
		set(DependenciesLibPath ${SolutionDir}/Dependencies/lib ${SolutionDir}/_Out/Release/)
	endif()
else()
	set(DependenciesLibPath ${SolutionDir}/Dependencies/lib ${SolutionDir}/_Out/)
endif()



link_directories(${DependenciesLibPath})

if(${BUILD_TOOLS_ONLY} STREQUAL "ON")
	message("Build Tools Only")
	add_subdirectory(NFTools)
else()
	message("Build All")
	add_subdirectory(NFTools)
	add_subdirectory(Dependencies)
	add_subdirectory(NFComm)
	add_subdirectory(NFServer)
	add_subdirectory(NFMidWare)
	add_subdirectory(Tutorial)
endif()

