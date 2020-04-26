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

if(UNIX)

	target_link_libraries(NFConsumeManagerPlugin -static-libstdc++)

	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

	target_link_libraries(NFConsumeManagerPlugin -static-libstdc++)

	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()