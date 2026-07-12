set(ProjectName "NFLoginNet_HttpServerPlugin")
file(GLOB NFLoginNet_HttpServerPlugin_ROOT_Cpp *.cpp)
file(GLOB NFLoginNet_HttpServerPlugin_ROOT_Hpp *.h)

add_library(NFLoginNet_HttpServerPlugin STATIC
	${NFLoginNet_HttpServerPlugin_ROOT_Cpp}
	${NFLoginNet_HttpServerPlugin_ROOT_Hpp})
	
set_target_properties( NFLoginNet_HttpServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLoginNet_HttpServerPlugin")
set_target_properties( NFLoginNet_HttpServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFLoginNet_HttpServerPlugin PROPERTIES 
	FOLDER "NFServer/NFLoginNet_HttpServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFLoginNet_HttpServerPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFLoginNet_HttpServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFLoginNet_HttpServerPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)
	
else()
	target_link_libraries(NFLoginNet_HttpServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFLoginNet_HttpServerPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)
	
endif()

