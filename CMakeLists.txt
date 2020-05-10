set_property(GLOBAL PROPERTY USE_FOLDERS On)

option(BUILD_TOOLS_ONLY "help string describing option" OFF)
option(BUILD_MID_WARE_SDK "help string describing option" OFF)

cmake_minimum_required(VERSION 2.8)

macro(link_NFSDK projName)
	add_dependencies(${projName} NFNetPlugin NFCore NFMessageDefine)
	target_link_libraries(${projName} NFCore NFMessageDefine NFNetPlugin)
endmacro(link_NFSDK)

project(NoahFrame)

#OSX上强制生成so后缀的动态库而不是dylib后缀动态库
if (APPLE)
	SET(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif(APPLE)

set(SolutionDir ${PROJECT_SOURCE_DIR})

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	set(NFOutPutDir ${PROJECT_SOURCE_DIR}/_Out/Debug)
else()
	set(NFOutPutDir ${PROJECT_SOURCE_DIR}/_Out/Release)
endif()


message("NFOutPutDir:${NFOutPutDir}")


# message(STATUS "SolutionDir=" ${SolutionDir})
if (APPLE)
	include_directories(
		${SolutionDir}
		${SolutionDir}/Dependencies/
		${SolutionDir}/Dependencies/NFMessageDefine/
		${SolutionDir}/NFComm/NFPluginModule/
		${SolutionDir}/Dependencies/hiredis/hiredis_linux
		${SolutionDir}/Dependencies/vcpkg/installed/x64-osx/include/
		${SolutionDir}/Dependencies/vcpkg/installed/x64-osx/include/SDL2
		${SolutionDir}/Dependencies/vcpkg/installed/x64-osx/include/google/protobuf/ 
		${SolutionDir}/Dependencies/vcpkg/installed/x64-osx/include/google/protobuf/src/
	)
elseif(UNIX)
	include_directories(
		${SolutionDir}
		${SolutionDir}/Dependencies/
		${SolutionDir}/Dependencies/NFMessageDefine/
		${SolutionDir}/NFComm/NFPluginModule/
		${SolutionDir}/Dependencies/hiredis/hiredis_linux
		${SolutionDir}/Dependencies/vcpkg/installed/x64-linux/include/
		${SolutionDir}/Dependencies/vcpkg/installed/x64-linux/include/SDL2
		${SolutionDir}/Dependencies/vcpkg/installed/x64-linux/include/google/protobuf/ 
		${SolutionDir}/Dependencies/vcpkg/installed/x64-linux/include/google/protobuf/src/
	)
else()
	include_directories(
		${SolutionDir}
		${SolutionDir}/Dependencies/
		${SolutionDir}/Dependencies/NFMessageDefine/
		${SolutionDir}/NFComm/NFPluginModule/
		${SolutionDir}/Dependencies/hiredis/hiredis_win
		${SolutionDir}/Dependencies/vcpkg/installed/x64-windows-static/include/
		${SolutionDir}/Dependencies/vcpkg/installed/x64-windows-static/include/SDL2
		${SolutionDir}/Dependencies/vcpkg/installed/x64-windows-static/include/google/protobuf/ 
		${SolutionDir}/Dependencies/vcpkg/installed/x64-windows-static/include/google/protobuf/src/
	)
endif()


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
	#MAC不需要这一行
	if (NOT APPLE)
		add_definitions(-std=c++11)
	endif()
else()
	SET ( CMAKE_CXX_FLAGS "-D_X64 -D_WINDOWS /EHsc" )
	include_directories(${SolutionDir}/Dependencies/libevent/WIN32-Code/)
endif()

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	set(DependenciesLibPath ${SolutionDir}/Dependencies/lib ${SolutionDir}/_Out/Debug/ ${SolutionDir}/Dependencies/lib/Debug)
else()
	set(DependenciesLibPath ${SolutionDir}/Dependencies/lib ${SolutionDir}/_Out/Release/ ${SolutionDir}/Dependencies/lib/Release)
endif()


link_directories(${DependenciesLibPath})

if(${BUILD_TOOLS_ONLY} STREQUAL "ON")
	message("Build Tools Only")
	add_subdirectory(NFTools)
elseif(${BUILD_MID_WARE_SDK} STREQUAL "ON")
	add_subdirectory(NFTools)
	message("Build SDK & MIDWARE")
	add_subdirectory(Dependencies)
	add_subdirectory(NFComm)
	add_subdirectory(NFExamples)
	add_subdirectory(NFServer)
	add_subdirectory(../NFMidWare NFMidWare)
	add_subdirectory(../NFComm/NFMessageDefineEx NFComm/NFMessageDefineEx)
	add_subdirectory(../NFComm/NFServerEx NFComm/NFServerEx)
else()
	message("Build SDK")
	add_subdirectory(Dependencies)
	add_subdirectory(NFComm)
	add_subdirectory(NFServer)
	add_subdirectory(Tutorial)
	add_subdirectory(NFExamples)
endif()
