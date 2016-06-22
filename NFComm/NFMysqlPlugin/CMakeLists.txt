set(ProjectName "NFMysqlClusterPlugin")
file(GLOB NFMysqlClusterPlugin_ROOT_Cpp *.cpp)
file(GLOB NFMysqlClusterPlugin_ROOT_CC *.cc)
file(GLOB NFMysqlClusterPlugin_ROOT_Hpp *.h)

add_library(NFMysqlClusterPlugin SHARED
	${NFMysqlClusterPlugin_ROOT_Cpp}
	${NFMysqlClusterPlugin_ROOT_CC}
	${NFMysqlClusterPlugin_ROOT_Hpp})
	
target_include_directories(NFMysqlClusterPlugin PUBLIC
	${SolutionDir}/Dependencies/mysql/
	${SolutionDir}/Dependencies/mysqlpp/)
	
if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFMysqlClusterPlugin libtheron.a libmysqlpp.a libmysqlclient.a)
	else()
		target_link_libraries(NFMysqlClusterPlugin libtherond.a libmysqlpp.a libmysqlclient.a)
	endif()
	add_definitions(
		-D_USRDLL
		-D_LIB
		-DPROTOBUF_USE_DLLS
		-DNFDATABASEPLUGIN_EXPORTS
		-DWIN32_LEAN_AND_MEAN
		)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_post_cmd_linux(${ProjectName} "NFLoginServer" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "NFWorldServer" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "NFMasterServer" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "NFProxyServer1" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "NFGameServer1" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial1" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial2" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial3" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial4" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial5" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial6" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial7" ".so" "" "lib")
	else()
		add_post_cmd_linux(${ProjectName} "NFLoginServer" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "NFWorldServer" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "NFMasterServer" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "NFProxyServer1" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "NFGameServer1" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial1" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial2" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial3" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial4" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial5" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial6" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial7" ".so" "_d" "lib")
	endif()
else()
	add_dependencies(NFMysqlClusterPlugin libprotobuf NFMessageDefine)
	target_link_libraries(NFMysqlClusterPlugin anet_win64 libprotobuf lua Theron NFMessageDefine)
	add_definitions(
		-D_USRDLL
		-D_LIB
		-DPROTOBUF_USE_DLLS
		-DNFDATABASEPLUGIN_EXPORTS
		-DWIN32_LEAN_AND_MEAN
		)
	
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_post_cmd_win(${ProjectName} "NFLoginServer" "dll" "")
		add_post_cmd_win(${ProjectName} "NFWorldServer" "dll" "")
		add_post_cmd_win(${ProjectName} "NFMasterServer" "dll" "")
		add_post_cmd_win(${ProjectName} "NFProxyServer1" "dll" "")
		add_post_cmd_win(${ProjectName} "NFGameServer1" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial1" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial2" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial3" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial4" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial5" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial6" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial7" "dll" "")
	else()
		add_post_cmd_win(${ProjectName} "NFLoginServer" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "NFWorldServer" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "NFMasterServer" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "NFProxyServer1" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "NFGameServer1" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "Tutorial1" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "Tutorial2" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "Tutorial3" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "Tutorial4" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "Tutorial5" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "Tutorial6" "dll" "_d" "lib")
		add_post_cmd_win(${ProjectName} "Tutorial7" "dll" "_d" "lib")
	endif()
endif()
set_target_properties( NFMysqlClusterPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFMysqlClusterPlugin_d")
set_target_properties( NFMysqlClusterPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )