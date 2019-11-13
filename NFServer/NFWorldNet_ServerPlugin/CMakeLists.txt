set(ProjectName "NFWorldNet_ServerPlugin")
file(GLOB NFWorldNet_ServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFWorldNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFWorldNet_ServerPlugin STATIC
	${NFWorldNet_ServerPlugin_ROOT_Cpp}
	${NFWorldNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFWorldNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFWorldNet_ServerPlugin")
set_target_properties( NFWorldNet_ServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFWorldNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/WorldServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK(NFWorldNet_ServerPlugin)

if(UNIX)
	target_link_libraries(NFWorldNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	target_link_libraries(NFWorldNet_ServerPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

endif()
