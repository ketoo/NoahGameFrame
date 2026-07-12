# NOTE: 修正 - 将 cmake_minimum_required 与 project() 提到文件首部（原来的顺序不符合规范）
# NOTE: CMake 最低版本从 2.8 升级到 3.20，兼容 CMake 4.x
cmake_minimum_required(VERSION 3.20)

project(NoahFrame LANGUAGES CXX C)

# NOTE: 以下 option / set_property / macro 原本散落在 cmake_minimum_required 之前，统一收拢到 project 之后
set_property(GLOBAL PROPERTY USE_FOLDERS On)

option(BUILD_TOOLS_ONLY "help string describing option" OFF)
option(BUILD_MID_WARE_SDK "help string describing option" OFF)

macro(link_NFSDK projName)
	add_dependencies(${projName} NFNetPlugin NFCore NFMessageDefine)
	# NOTE: 显式使用 PRIVATE 关键字，避免与其它带 keyword 的 target_link_libraries 冲突
	target_link_libraries(${projName} PRIVATE NFCore NFMessageDefine NFNetPlugin)
endmacro(link_NFSDK)

# NOTE: 使用现代 C++ 标准声明替代 add_definitions(-std=c++17) 与 CXX_FLAGS 中的 -std=c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#OSX上强制生成so后缀的动态库而不是dylib后缀动态库
if (APPLE)
	set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif()

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

# NOTE: 使用 add_compile_options + generator expression 替代旧的直接覆盖 CMAKE_CXX_FLAGS
if(UNIX)
	add_compile_options(-fPIC -Wall)
	add_compile_options(
		"$<$<CONFIG:Debug>:-O0;-g;-ggdb;-DDEBUG>"
		"$<$<CONFIG:Release>:-O3>"
	)

	if( CMAKE_BUILD_TYPE STREQUAL "" )
		set(CMAKE_BUILD_TYPE "Release")
	endif()

	set(LDFLAGS "$ENV{LDFLAGS} -stdlib=libc++")
elseif(MSVC)
	# NOTE: 使用 add_compile_definitions/add_compile_options 替代覆盖 CMAKE_CXX_FLAGS
	add_compile_definitions(_X64 _WINDOWS)
	add_compile_options(/EHsc)
	include_directories(${SolutionDir}/Dependencies/libevent/WIN32-Code/)
endif()

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	set(DependenciesLibPath ${SolutionDir}/Dependencies/lib ${SolutionDir}/_Out/Debug/ ${SolutionDir}/Dependencies/lib/Debug)
else()
	set(DependenciesLibPath ${SolutionDir}/Dependencies/lib ${SolutionDir}/_Out/Release/ ${SolutionDir}/Dependencies/lib/Release)
endif()

link_directories(${DependenciesLibPath})

# NOTE: 简化 option 判定，去掉 ${VAR} STREQUAL "ON" 的老式写法
if(BUILD_TOOLS_ONLY)
	message("Build Tools Only")
	add_subdirectory(NFTools)
elseif(BUILD_MID_WARE_SDK)
	add_subdirectory(NFTools)
	message("Build SDK & MIDWARE")
	add_subdirectory(Dependencies)
	add_subdirectory(NFComm)
	add_subdirectory(NFServer)
	add_subdirectory(Tutorial)
	add_subdirectory(NFExamples)
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
