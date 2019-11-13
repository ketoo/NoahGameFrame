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
	FOLDER "NFMidWare/NFTestPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFTestPlugin")
if(UNIX)
	target_include_directories(NFTestPlugin PUBLIC
			${SolutionDir}/Dependencies/googletest-release-1.8.0/googletest/include/)


	target_link_libraries(NFTestPlugin -static-libstdc++)

	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

	target_link_libraries(NFTestPlugin -static-libstdc++)
	
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
