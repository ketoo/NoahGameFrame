set(ProjectName "NFLoginNet_ClientPlugin")
file(GLOB NFLoginNet_ClientPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFLoginNet_ClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFLoginNet_ClientPlugin STATIC
	${NFLoginNet_ClientPlugin_ROOT_Cpp}
	${NFLoginNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFLoginNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLoginNet_ClientPlugin")
set_target_properties( NFLoginNet_ClientPlugin PROPERTIES PREFIX "")
set_target_properties( NFLoginNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/NFLoginNet_ClientPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFLoginNet_ClientPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFLoginNet_ClientPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFLoginNet_ClientPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(NFLoginNet_ClientPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFLoginNet_ClientPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)

endif()