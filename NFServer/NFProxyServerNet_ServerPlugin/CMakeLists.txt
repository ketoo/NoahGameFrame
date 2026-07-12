set(ProjectName "NFProxyServerNet_ServerPlugin")
file(GLOB NFProxyServerNet_ServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFProxyServerNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFProxyServerNet_ServerPlugin STATIC
	${NFProxyServerNet_ServerPlugin_ROOT_Cpp}
	${NFProxyServerNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFProxyServerNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFProxyServerNet_ServerPlugin")
set_target_properties( NFProxyServerNet_ServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFProxyServerNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/NFProxyServerNet_ServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFProxyServerNet_ServerPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFProxyServerNet_ServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFProxyServerNet_ServerPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(NFProxyServerNet_ServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFProxyServerNet_ServerPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)

endif()

