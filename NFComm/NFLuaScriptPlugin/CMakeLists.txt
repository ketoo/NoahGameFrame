set(ProjectName "NFLuaScriptPlugin")
file(GLOB NFLuaScriptPlugin_ROOT_Cpp *.cpp)
file(GLOB NFLuaScriptPlugin_ROOT_Hpp *.h)

add_library(NFLuaScriptPlugin STATIC
	${NFLuaScriptPlugin_ROOT_Cpp}
	${NFLuaScriptPlugin_ROOT_Hpp})

if(UNIX)
	add_dependencies(NFLuaScriptPlugin NFCore)
	
	if (CMAKE_BUILD_TYPE MATCHES "Release")
	target_link_libraries(NFLuaScriptPlugin liblua.a)
	target_link_libraries(NFLuaScriptPlugin libprotobuf.a)
	else()
	target_link_libraries(NFLuaScriptPlugin libprotobuf.a)
	target_link_libraries(NFLuaScriptPlugin liblua.a)
	endif()
	
	target_link_libraries(NFLuaScriptPlugin -static-libstdc++)
else()
	add_dependencies(NFLuaScriptPlugin NFCore )
	target_link_libraries(NFLuaScriptPlugin libprotobuf )
	target_link_libraries(NFLuaScriptPlugin liblua.a)

	target_link_libraries(NFLuaScriptPlugin -static-libstdc++)

endif()

set_target_properties( NFLuaScriptPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLuaScriptPlugin_d")
set_target_properties( NFLuaScriptPlugin PROPERTIES PREFIX "")
set_target_properties( NFLuaScriptPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )