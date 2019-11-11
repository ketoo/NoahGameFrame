set(ProjectName "NFMasterNet_ServerPlugin")
file(GLOB NFMasterNet_ServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFMasterNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFMasterNet_ServerPlugin STATIC
	${NFMasterNet_ServerPlugin_ROOT_Cpp}
	${NFMasterNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFMasterNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFMasterNet_ServerPlugin")
set_target_properties( NFMasterNet_ServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFMasterNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/MasterServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFMasterNet_ServerPlugin")

if(UNIX)
	target_link_libraries(NFMasterNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	target_link_libraries(NFMasterNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

endif()