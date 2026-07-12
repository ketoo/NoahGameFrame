set(ProjectName "NFMasterNet_HttpServerPlugin")
file(GLOB NFMasterNet_HttpServerPlugin_ROOT_Cpp *.cpp)
file(GLOB NFMasterNet_HttpServerPlugin_ROOT_Hpp *.h)

add_library(NFMasterNet_HttpServerPlugin STATIC
	${NFMasterNet_HttpServerPlugin_ROOT_Cpp}
	${NFMasterNet_HttpServerPlugin_ROOT_Hpp})
	
set_target_properties( NFMasterNet_HttpServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFMasterNet_HttpServerPlugin")
set_target_properties( NFMasterNet_HttpServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFMasterNet_HttpServerPlugin PROPERTIES 
	FOLDER "NFServer/NFMasterNet_HttpServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFMasterNet_HttpServerPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFMasterNet_HttpServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFMasterNet_HttpServerPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)
	
else()
	target_link_libraries(NFMasterNet_HttpServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFMasterNet_HttpServerPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)
	
endif()

