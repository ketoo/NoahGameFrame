file(GLOB NFGameServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFGameServerPlugin_ROOT_Hpp 
	*.h)

remove_file(NFGameServerPlugin_ROOT_Cpp "NFCCreateRoleModule.cpp")	
remove_file(NFGameServerPlugin_ROOT_Hpp "NFCCreateRoleModule.h")	
remove_file(NFGameServerPlugin_ROOT_Cpp "NFCEventRouteModule.cpp")	
remove_file(NFGameServerPlugin_ROOT_Hpp "NFCEventRouteModule.h")	

add_library(NFGameServerPlugin SHARED
	${NFGameServerPlugin_ROOT_Cpp}
	${NFGameServerPlugin_ROOT_Hpp})
	
set_target_properties( NFGameServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGameServerPlugin_d")
set_target_properties( NFGameServerPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("NFGameServerPlugin")

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