set(ProjectName "NFWorldNet_ClientPlugin")
file(GLOB NFWorldNet_ClientPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFWorldNet_ClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFWorldNet_ClientPlugin STATIC
	${NFWorldNet_ClientPlugin_ROOT_Cpp}
	${NFWorldNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFWorldNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFWorldNet_ClientPlugin")
set_target_properties( NFWorldNet_ClientPlugin PROPERTIES PREFIX "")
set_target_properties( NFWorldNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/NFWorldNet_ClientPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFWorldNet_ClientPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFWorldNet_ClientPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFWorldNet_ClientPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(NFWorldNet_ClientPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFWorldNet_ClientPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)

endif()

