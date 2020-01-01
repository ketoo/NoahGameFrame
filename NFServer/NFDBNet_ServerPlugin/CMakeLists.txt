set(ProjectName "NFDBNet_ServerPlugin")
file(GLOB NFDBNet_ServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFDBNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFDBNet_ServerPlugin STATIC
	${NFDBNet_ServerPlugin_ROOT_Cpp}
	${NFDBNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFDBNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFDBNet_ServerPlugin")
set_target_properties( NFDBNet_ServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFDBNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/NFDBNet_ServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK(NFDBNet_ServerPlugin)

if(UNIX)
	target_link_libraries(NFDBNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	target_link_libraries(NFDBNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-DELPP_NO_DEFAULT_LOG_FILE
	)

endif()
