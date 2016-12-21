set(ProjectName "NFProxyLogicPlugin")
file(GLOB NFProxyLogicPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFProxyLogicPlugin_ROOT_Hpp 
	*.h)

add_library(NFProxyLogicPlugin SHARED
	${NFProxyLogicPlugin_ROOT_Cpp}
	${NFProxyLogicPlugin_ROOT_Hpp})
	
set_target_properties( NFProxyLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFProxyLogicPlugin_d")
set_target_properties( NFProxyLogicPlugin PROPERTIES PREFIX "")
set_target_properties( NFProxyLogicPlugin PROPERTIES 
	FOLDER "NFServer/ProxyServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFProxyLogicPlugin")

if(UNIX)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
endif()
