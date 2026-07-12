set(ProjectName "NFProxyServerNet_ClientPlugin")
file(GLOB NFProxyServerNet_ClientPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFProxyServerNet_ClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFProxyServerNet_ClientPlugin STATIC
	${NFProxyServerNet_ClientPlugin_ROOT_Cpp}
	${NFProxyServerNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFProxyServerNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFProxyServerNet_ClientPlugin")
set_target_properties( NFProxyServerNet_ClientPlugin PROPERTIES PREFIX "")
set_target_properties( NFProxyServerNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/NFProxyServerNet_ClientPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFProxyServerNet_ClientPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFProxyServerNet_ClientPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFProxyServerNet_ClientPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(NFProxyServerNet_ClientPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFProxyServerNet_ClientPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)

endif()

