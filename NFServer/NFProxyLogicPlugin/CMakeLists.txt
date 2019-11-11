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
	FOLDER "NFServer/ProxyServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFProxyLogicPlugin")

if(UNIX)
	target_link_libraries(NFProxyLogicPlugin -static-libstdc++)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	target_link_libraries(NFProxyLogicPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
endif()
