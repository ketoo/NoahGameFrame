set(ProjectName "NFTestPlugin")
file(GLOB NFTestPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFTestPlugin_ROOT_Hpp 
	*.h)

add_library(NFTestPlugin STATIC
	${NFTestPlugin_ROOT_Cpp}
	${NFTestPlugin_ROOT_Hpp})
	
set_target_properties( NFTestPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFTestPlugin")
set_target_properties( NFTestPlugin PROPERTIES PREFIX "")
set_target_properties( NFTestPlugin PROPERTIES 
	FOLDER "NFComm/NFTestPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

link_NFSDK("NFTestPlugin")

# NOTE: target_link_libraries 加 PRIVATE；add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(NFTestPlugin PRIVATE -static-libstdc++)

	target_compile_definitions(NFTestPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)
	
else()

	target_link_libraries(NFTestPlugin PRIVATE -static-libstdc++)
	
	target_compile_definitions(NFTestPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)
endif()
