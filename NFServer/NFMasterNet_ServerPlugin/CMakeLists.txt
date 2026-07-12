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
	FOLDER "NFServer/NFMasterNet_ServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFMasterNet_ServerPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFMasterNet_ServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFMasterNet_ServerPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(NFMasterNet_ServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFMasterNet_ServerPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)

endif()