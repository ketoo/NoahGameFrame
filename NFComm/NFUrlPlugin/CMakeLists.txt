set(ProjectName "NFUrlPlugin")
if(UNIX)
	
else()
	file(GLOB NFUrlPlugin_ROOT_Cpp *.cpp)
	file(GLOB NFUrlPlugin_ROOT_Hpp *.h)

	remove_file(NFUrlPlugin_ROOT_Cpp "Plugin.cpp")

	add_library(NFUrlPlugin SHARED
	${NFUrlPlugin_ROOT_Cpp}
	${NFUrlPlugin_ROOT_Hpp})
	add_dependencies(NFUrlPlugin libprotobuf NFCore NFMessageDefine)
	target_link_libraries(NFUrlPlugin anet_win64 libprotobuf lua Theron NFMessageDefine)
	add_definitions("-DWIN -D_USRDLL -DCURL_STATICLIB -DHTTP_ONLY")
	
	set_target_properties( NFUrlPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFUrlPlugin_d")
	set_target_properties( NFUrlPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
	
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
		add_post_cmd_win(${ProjectName} "NFLoginServer" "dll" "_d")
		add_post_cmd_win(${ProjectName} "NFWorldServer" "dll" "_d")
		add_post_cmd_win(${ProjectName} "NFMasterServer" "dll" "_d")
		add_post_cmd_win(${ProjectName} "NFProxyServer1" "dll" "_d")
		add_post_cmd_win(${ProjectName} "NFGameServer1" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial1" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial2" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial3" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial4" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial5" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial6" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial7" "dll" "_d")
	endif()
endif()