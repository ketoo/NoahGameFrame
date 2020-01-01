set(ProjectName "NFLuaScriptPlugin")
file(GLOB NFLuaScriptPlugin_ROOT_Cpp *.cpp)
file(GLOB NFLuaScriptPlugin_ROOT_Hpp *.h)

add_library(NFLuaScriptPlugin STATIC
	${NFLuaScriptPlugin_ROOT_Cpp}
	${NFLuaScriptPlugin_ROOT_Hpp})

target_link_libraries(NFLuaScriptPlugin liblua.a)
add_dependencies(NFLuaScriptPlugin NFCore)

if(UNIX)

	target_link_libraries(NFLuaScriptPlugin -static-libstdc++)
else()


endif()

set_target_properties( NFLuaScriptPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLuaScriptPlugin")
set_target_properties( NFLuaScriptPlugin PROPERTIES PREFIX "")
set_target_properties( NFLuaScriptPlugin PROPERTIES
	FOLDER "NFComm/NFLuaScriptPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )