set(ProjectName "NFWorldNet_ServerPlugin")
file(GLOB NFWorldNet_ServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFWorldNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFWorldNet_ServerPlugin STATIC
	${NFWorldNet_ServerPlugin_ROOT_Cpp}
	${NFWorldNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFWorldNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFWorldNet_ServerPlugin")
set_target_properties( NFWorldNet_ServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFWorldNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/NFWorldNet_ServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK(NFWorldNet_ServerPlugin)

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFWorldNet_ServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFWorldNet_ServerPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(NFWorldNet_ServerPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFWorldNet_ServerPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)

endif()

