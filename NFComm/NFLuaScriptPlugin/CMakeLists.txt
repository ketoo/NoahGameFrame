file(GLOB NFLuaScriptPlugin_ROOT_Cpp *.cpp)
file(GLOB NFLuaScriptPlugin_ROOT_Hpp *.h)

add_library(NFLuaScriptPlugin SHARED
	${NFLuaScriptPlugin_ROOT_Cpp}
	${NFLuaScriptPlugin_ROOT_Hpp})


if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	add_dependencies(NFLuaScriptPlugin NFConfigPlugin NFCore NFKernelPlugin NFLogPlugin NFMessageDefine)
	add_definitions("-D_USRDLL -DELPP_NO_DEFAULT_LOG_FILE")
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFLuaScriptPlugin libtheron.a lua)
	else()
		target_link_libraries(NFLuaScriptPlugin libtherond.a lua_d)
	endif()
else()
	add_dependencies(NFLuaScriptPlugin libprotobuf lua NFConfigPlugin NFCore NFKernelPlugin NFLogPlugin NFMessageDefine)
	target_link_libraries(NFLuaScriptPlugin anet_win64 libprotobuf lua Theron)
	add_definitions("-D_USRDLL -DTHERON_CPP11")
endif()
set_target_properties( NFLuaScriptPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLuaScriptPlugin_d")
set_target_properties( NFLuaScriptPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )



