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

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFMasterServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFMasterServerPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(NFMasterServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFMasterServerPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)

endif()