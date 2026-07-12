set(ProjectName "NFLoginLogicPlugin")
file(GLOB NFLoginLogicPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFLoginLogicPlugin_ROOT_Hpp 
	*.h)

add_library(NFLoginLogicPlugin STATIC
	${NFLoginLogicPlugin_ROOT_Cpp}
	${NFLoginLogicPlugin_ROOT_Hpp})
	
set_target_properties( NFLoginLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLoginLogicPlugin")
set_target_properties( NFLoginLogicPlugin PROPERTIES PREFIX "")
set_target_properties( NFLoginLogicPlugin PROPERTIES 
	FOLDER "NFServer/NFLoginLogicPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFLoginLogicPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFLoginLogicPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFLoginLogicPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(NFLoginLogicPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFLoginLogicPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)

endif()