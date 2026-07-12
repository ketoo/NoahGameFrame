set(ProjectName "NFDBLogicPlugin")
file(GLOB NFDataAgent_NosqlPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFDataAgent_NosqlPlugin_ROOT_Hpp 
	*.h)

add_library(NFDBLogicPlugin STATIC
	${NFDataAgent_NosqlPlugin_ROOT_Cpp}
	${NFDataAgent_NosqlPlugin_ROOT_Hpp})
	
set_target_properties( NFDBLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFDBLogicPlugin")
set_target_properties( NFDBLogicPlugin PROPERTIES PREFIX "")
set_target_properties( NFDBLogicPlugin PROPERTIES 
	FOLDER "NFServer/NFDBLogicPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFDBLogicPlugin")

# NOTE: target_link_libraries 添加 PRIVATE，add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFDBLogicPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFDBLogicPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)
else()
	target_link_libraries(NFDBLogicPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFDBLogicPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)
endif()