set_property(GLOBAL PROPERTY USE_FOLDERS On)

option(BUILD_TOOLS_ONLY "help string describing option")

cmake_minimum_required(VERSION 2.8)
macro(remove_file arg_list file_remove)
	file(GLOB RemoveItems_Cpp ${file_remove})
	list(REMOVE_ITEM ${arg_list} ${RemoveItems_Cpp})
endmacro(remove_file)

macro(link_NFSDK projName)
if(UNIX)
	add_dependencies(${projName} NFNet NFCore NFMessageDefine)
	target_link_libraries(${projName} protobuf NFMessageDefine NFNet event event_core)
else()
	add_dependencies(${projName} NFNet NFCore NFMessageDefine)
endif()
endmacro(link_NFSDK)

project(NoahFrame)

set(SolutionDir ${PROJECT_SOURCE_DIR})
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
)

#message("Build Type:"${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS})

if(UNIX)
	if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -Wall -g -ggdb -DDEBUG")
	else()
		set(CMAKE_BUILD_TYPE "Release")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11 -O3 -Wall")
	endif()
	set(LDFLAGS "$ENV{LDFLAGS} -stdlib=libc++")
	add_definitions(-std=c++11)
else()
	SET ( CMAKE_CXX_FLAGS "-D_X64 -D_WINDOWS" )
	include_directories(${SolutionDir}/Dependencies/libevent/WIN32-Code/)
endif()
set(DependenciesLibPath ${SolutionDir}/Dependencies/lib ${SolutionDir}/_Out/Comm )
link_directories(${DependenciesLibPath})

if(${BUILD_TOOLS_ONLY} STREQUAL "ON")
	message("Build Tools Only")
	add_subdirectory(NFTools)
else()
	message("Build All")
	add_subdirectory(Dependencies)
	add_subdirectory(NFComm)
	add_subdirectory(NFServer)
	add_subdirectory(Tutorial)
	add_subdirectory(NFTools)
endif()

