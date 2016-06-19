file(GLOB NFGameServerNet_ClientPlugin_ROOT_Cpp
	dllmain.cpp
	NFCGameServerToWorldModule.cpp
	NFGameServerNet_ClientPlugin.cpp)
file(GLOB NFGameServerNet_ClientPlugin_ROOT_Hpp 
	NFCGameServerToWorldModule.h
	NFGameServerNet_ClientPlugin.h)

add_library(NFGameServerNet_ClientPlugin SHARED
	${NFGameServerNet_ClientPlugin_ROOT_Cpp}
	${NFGameServerNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFGameServerNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGameServerNet_ClientPlugin_d")
set_target_properties( NFGameServerNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )

link_NFSDK("NFGameServerNet_ClientPlugin")

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