set(ProjectName "NFGameServerNet_ClientPlugin")
file(GLOB NFGameServerNet_ClientPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFGameServerNet_ClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFGameServerNet_ClientPlugin STATIC
	${NFGameServerNet_ClientPlugin_ROOT_Cpp}
	${NFGameServerNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFGameServerNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGameServerNet_ClientPlugin")
set_target_properties( NFGameServerNet_ClientPlugin PROPERTIES PREFIX "")
set_target_properties( NFGameServerNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/NFGameServerNet_ClientPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

link_NFSDK("NFGameServerNet_ClientPlugin")

if(UNIX)

	target_link_libraries(NFGameServerNet_ClientPlugin -static-libstdc++)
	add_definitions(
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	

else()


endif()