set(ProjectName "NFProxyLogicPlugin")
file(GLOB NFProxyLogicPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFProxyLogicPlugin_ROOT_Hpp 
	*.h)

add_library(NFProxyLogicPlugin STATIC
	${NFProxyLogicPlugin_ROOT_Cpp}
	${NFProxyLogicPlugin_ROOT_Hpp})
	
set_target_properties( NFProxyLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFProxyLogicPlugin")
set_target_properties( NFProxyLogicPlugin PROPERTIES PREFIX "")
set_target_properties( NFProxyLogicPlugin PROPERTIES 
	FOLDER "NFServer/NFProxyLogicPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFProxyLogicPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFProxyLogicPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFProxyLogicPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(NFProxyLogicPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFProxyLogicPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)
	
endif()

