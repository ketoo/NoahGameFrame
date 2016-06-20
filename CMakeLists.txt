set_property(GLOBAL PROPERTY USE_FOLDERS On)
cmake_minimum_required(VERSION 2.8)

macro(remove_file arg_list file_remove)
	file(GLOB RemoveItems_Cpp ${file_remove})
	list(REMOVE_ITEM ${arg_list} ${RemoveItems_Cpp})
endmacro(remove_file)

macro(link_NFSDK projName)
	#target_link_libraries(${projName} NFConfigPlugin NFCore NFKernelPlugin NFLogPlugin NFLuaScriptPlugin NFMessageDefine NFNet NFUrlPlugin)
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

if(UNIX)
	set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS}  -std=c++11 -O0 -Wall -g -ggdb -fPIC -DDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS}  -std=c++11 -O3 -Wall -fPIC")
	set(LDFLAGS "$ENV{LDFLAGS} -stdlib=libc++")
	add_definitions(-std=c++11)
else()
	SET ( CMAKE_CXX_FLAGS "-D_X64 -D_WINDOWS" )
	include_directories(${SolutionDir}/Dependencies/libevent/WIN32-Code/)
endif()
set(DependenciesLibPath ${SolutionDir}/Dependencies/lib ${SolutionDir}/_Out/Comm )
link_directories(${DependenciesLibPath})

add_subdirectory(Dependencies)
add_subdirectory(NFComm)
add_subdirectory(NFServer)
add_subdirectory(Tutorial)
add_subdirectory(NFTools)

