set(ProjectName "NFGameLogicPlugin")
file(GLOB NFGameLogicPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFGameLogicPlugin_ROOT_Hpp 
	*.h)
	
add_library(NFGameLogicPlugin SHARED
	${NFGameLogicPlugin_ROOT_Cpp}
	${NFGameLogicPlugin_ROOT_Hpp})
	
set_target_properties( NFGameLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGameLogicPlugin_d")
set_target_properties( NFGameLogicPlugin PROPERTIES PREFIX "")
set_target_properties( NFGameLogicPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFGameLogicPlugin")

if(UNIX)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
else()
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()