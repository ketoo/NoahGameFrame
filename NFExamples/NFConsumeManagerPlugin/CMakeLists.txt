set(ProjectName "NFConsumeManagerPlugin")
file(GLOB ConsumeManagerPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB ConsumeManager_ROOT_Hpp
	*.h)

add_library(NFConsumeManagerPlugin STATIC
	${ConsumeManagerPlugin_ROOT_Cpp}
	${ConsumeManager_ROOT_Hpp})
	
set_target_properties( NFConsumeManagerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFConsumeManagerPlugin")
set_target_properties( NFConsumeManagerPlugin PROPERTIES PREFIX "")
set_target_properties( NFConsumeManagerPlugin PROPERTIES
	FOLDER "NFExamples/NFConsumeManagerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

link_NFSDK("NFConsumeManagerPlugin")

# NOTE: target_link_libraries 添加 PRIVATE；add_definitions 转 target_compile_definitions
if(UNIX)

	target_link_libraries(NFConsumeManagerPlugin PRIVATE -static-libstdc++)

	target_compile_definitions(NFConsumeManagerPlugin PRIVATE _USRDLL ELPP_NO_DEFAULT_LOG_FILE)
	
else()

	target_link_libraries(NFConsumeManagerPlugin PRIVATE -static-libstdc++)

	target_compile_definitions(NFConsumeManagerPlugin PRIVATE WIN _USRDLL ELPP_NO_DEFAULT_LOG_FILE)
endif()