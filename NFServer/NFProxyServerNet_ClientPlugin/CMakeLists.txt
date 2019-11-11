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
	FOLDER "NFServer/ProxyServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFProxyServerNet_ClientPlugin")

if(UNIX)
	target_link_libraries(NFProxyServerNet_ClientPlugin -static-libstdc++)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	target_link_libraries(NFProxyServerNet_ClientPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

endif()
