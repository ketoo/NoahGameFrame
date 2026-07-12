set(ProjectName "NFGameServerPlugin")
file(GLOB NFGameServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFGameServerPlugin_ROOT_Hpp 
	*.h)	

add_library(NFGameServerPlugin STATIC
	${NFGameServerPlugin_ROOT_Cpp}
	${NFGameServerPlugin_ROOT_Hpp})
	
set_target_properties( NFGameServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGameServerPlugin")
set_target_properties( NFGameServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFGameServerPlugin PROPERTIES 
	FOLDER "NFServer/NFGameServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFGameServerPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFGameServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFGameServerPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)
	
else()
	target_link_libraries(NFGameServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFGameServerPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)
	
endif()