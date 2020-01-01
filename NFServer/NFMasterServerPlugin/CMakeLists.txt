set(ProjectName "NFMasterServerPlugin")
file(GLOB NFMasterServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFMasterServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFMasterServerPlugin STATIC
	${NFMasterServerPlugin_ROOT_Cpp}
	${NFMasterServerPlugin_ROOT_Hpp})
	
set_target_properties( NFMasterServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFMasterServerPlugin")
set_target_properties( NFMasterServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFMasterServerPlugin PROPERTIES 
	FOLDER "NFServer/NFMasterServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFMasterServerPlugin")

if(UNIX)
	target_link_libraries(NFMasterServerPlugin -static-libstdc++)
	add_definitions(
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	target_link_libraries(NFMasterServerPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-DELPP_NO_DEFAULT_LOG_FILE
	)

endif()