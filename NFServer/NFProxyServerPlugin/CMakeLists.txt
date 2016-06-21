file(GLOB NFProxyServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFProxyServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFProxyServerPlugin SHARED
	${NFProxyServerPlugin_ROOT_Cpp}
	${NFProxyServerPlugin_ROOT_Hpp})
	
set_target_properties( NFProxyServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFProxyServerPlugin_d")
set_target_properties( NFProxyServerPlugin PROPERTIES 
	FOLDER "NFServer/ProxyServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("NFProxyServerPlugin")

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
