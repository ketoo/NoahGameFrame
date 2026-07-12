set(ProjectName "NFDBNet_ClientPlugin")
file(GLOB NFDBNet_ClientPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFDBNet_ClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFDBNet_ClientPlugin STATIC
	${NFDBNet_ClientPlugin_ROOT_Cpp}
	${NFDBNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFDBNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFDBNet_ClientPlugin")
set_target_properties( NFDBNet_ClientPlugin PROPERTIES PREFIX "")
set_target_properties( NFDBNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/NFDBNet_ClientPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFDBNet_ClientPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFDBNet_ClientPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFDBNet_ClientPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(NFDBNet_ClientPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFDBNet_ClientPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)

endif()

