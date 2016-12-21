set(ProjectName "NFGameServerNet_ClientPlugin")
file(GLOB NFGameServerNet_ClientPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFGameServerNet_ClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFGameServerNet_ClientPlugin SHARED
	${NFGameServerNet_ClientPlugin_ROOT_Cpp}
	${NFGameServerNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFGameServerNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGameServerNet_ClientPlugin_d")
set_target_properties( NFGameServerNet_ClientPlugin PROPERTIES PREFIX "")
set_target_properties( NFGameServerNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

link_NFSDK("NFGameServerNet_ClientPlugin")

if(UNIX)
	target_link_libraries(NFGameServerNet_ClientPlugin protobuf)
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