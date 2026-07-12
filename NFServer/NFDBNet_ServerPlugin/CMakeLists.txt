set(ProjectName "NFDBNet_ServerPlugin")
file(GLOB NFDBNet_ServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFDBNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFDBNet_ServerPlugin STATIC
	${NFDBNet_ServerPlugin_ROOT_Cpp}
	${NFDBNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFDBNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFDBNet_ServerPlugin")
set_target_properties( NFDBNet_ServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFDBNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/NFDBNet_ServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK(NFDBNet_ServerPlugin)

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFDBNet_ServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFDBNet_ServerPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(NFDBNet_ServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFDBNet_ServerPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)

endif()

