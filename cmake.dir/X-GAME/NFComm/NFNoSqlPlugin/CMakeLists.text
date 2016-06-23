set(ProjectName "NFNoSqlPlugin")
file(GLOB NFNoSqlPlugin_ROOT_Cpp *.cpp)
file(GLOB NFNoSqlPlugin_ROOT_CC *.cc)
file(GLOB NFNoSqlPlugin_ROOT_Hpp *.h)

add_library(NFNoSqlPlugin SHARED
	${NFNoSqlPlugin_ROOT_Cpp}
	${NFNoSqlPlugin_ROOT_CC}
	${NFNoSqlPlugin_ROOT_Hpp}
	${SolutionDir}/Dependencies/redis-cplusplus-client/anet.c
	${SolutionDir}/Dependencies/redis-cplusplus-client/anet.h)
	
if(UNIX)
	target_include_directories(NFNoSqlPlugin PUBLIC
		${SolutionDir}/Dependencies/redis-cplusplus-client/)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFNoSqlPlugin libtheron.a)
	else()
		target_link_libraries(NFNoSqlPlugin libtherond.a)
	endif()
	add_definitions(
			-D_USRDLL
			-DTHERON_CPP11
		)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_post_cmd_linux(${ProjectName} "NFLoginServer" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "NFWorldServer" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "NFGameServer1" ".so" "" "lib")
	else()
		add_post_cmd_linux(${ProjectName} "NFLoginServer" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "NFWorldServer" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "NFGameServer1" ".so" "_d" "lib")
	endif()
else()
	target_include_directories(NFNoSqlPlugin PUBLIC
		${SolutionDir}/Dependencies/redis-cplusplus-client/msvc/anet_win32/)
	add_dependencies(NFNoSqlPlugin libprotobuf NFMessageDefine)
	target_link_libraries(NFNoSqlPlugin anet_win64 libprotobuf lua Theron NFMessageDefine)
	add_definitions(
			-D_USRDLL
			-DTHERON_CPP11
		)
	
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_post_cmd_win(${ProjectName} "NFLoginServer" "dll" "")
		add_post_cmd_win(${ProjectName} "NFWorldServer" "dll" "")
		add_post_cmd_win(${ProjectName} "NFGameServer1" "dll" "")
	else()
		add_post_cmd_win(${ProjectName} "NFLoginServer" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "NFWorldServer" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "NFGameServer1" "dll" "_d" "lib")
	endif()
endif()
set_target_properties( NFNoSqlPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFNoSqlPlugin_d")
set_target_properties( NFNoSqlPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )