set(ProjectName "NFLoginNet_ServerPlugin")
file(GLOB NFLoginNet_ServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFLoginNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFLoginNet_ServerPlugin STATIC
	${NFLoginNet_ServerPlugin_ROOT_Cpp}
	${NFLoginNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFLoginNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLoginNet_ServerPlugin")
set_target_properties( NFLoginNet_ServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFLoginNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/NFLoginNet_ServerPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFLoginNet_ServerPlugin")

if(UNIX)
	target_link_libraries(NFLoginNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	target_link_libraries(NFLoginNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-DELPP_NO_DEFAULT_LOG_FILE
	)

endif()