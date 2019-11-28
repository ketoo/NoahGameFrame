set(ProjectName "NFBluePrintPlugin")
file(GLOB NFBluePrintPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFBluePrintPlugin_ROOT_Hpp 
	*.h)

add_library(NFBluePrintPlugin STATIC
	${NFBluePrintPlugin_ROOT_Cpp}
	${NFBluePrintPlugin_ROOT_Hpp})
	
set_target_properties( NFBluePrintPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFBluePrintPlugin")
set_target_properties( NFBluePrintPlugin PROPERTIES PREFIX "")
set_target_properties( NFBluePrintPlugin PROPERTIES 
	FOLDER "NFComm/NFBluePrintPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFBluePrintPlugin")
if(UNIX)
	target_include_directories(NFBluePrintPlugin PUBLIC
			${SolutionDir}/Dependencies/googletest-release-1.8.0/googletest/include/)


	target_link_libraries(NFBluePrintPlugin -static-libstdc++)

	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

	target_link_libraries(NFBluePrintPlugin -static-libstdc++)
	
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
