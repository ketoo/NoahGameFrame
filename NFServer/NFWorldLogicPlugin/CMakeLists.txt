set(ProjectName "NFWorldLogicPlugin")
file(GLOB NFWorldLogicPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFWorldLogicPlugin_ROOT_Hpp 
	*.h)

add_library(NFWorldLogicPlugin STATIC
	${NFWorldLogicPlugin_ROOT_Cpp}
	${NFWorldLogicPlugin_ROOT_Hpp})
	
set_target_properties( NFWorldLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFWorldLogicPlugin")
set_target_properties( NFWorldLogicPlugin PROPERTIES PREFIX "")
set_target_properties( NFWorldLogicPlugin PROPERTIES 
	FOLDER "NFServer/WorldServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFWorldLogicPlugin")

if(UNIX)
	target_link_libraries(NFWorldLogicPlugin -static-libstdc++)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	target_link_libraries(NFWorldLogicPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

endif()
