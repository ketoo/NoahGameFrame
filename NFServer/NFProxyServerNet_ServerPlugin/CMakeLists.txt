file(GLOB NFProxyServerNet_ServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFProxyServerNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFProxyServerNet_ServerPlugin SHARED
	${NFProxyServerNet_ServerPlugin_ROOT_Cpp}
	${NFProxyServerNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFProxyServerNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFProxyServerNet_ServerPlugin_d")
set_target_properties( NFProxyServerNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/ProxyServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("NFProxyServerNet_ServerPlugin")

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
