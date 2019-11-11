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
	FOLDER "NFServer/ProxyServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFProxyServerNet_ServerPlugin")

if(UNIX)
	target_link_libraries(NFProxyServerNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	target_link_libraries(NFProxyServerNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

endif()
