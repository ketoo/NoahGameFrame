set(ProjectName "NFDBLogicPlugin")
file(GLOB NFDataAgent_NosqlPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFDataAgent_NosqlPlugin_ROOT_Hpp 
	*.h)

add_library(NFDBLogicPlugin STATIC
	${NFDataAgent_NosqlPlugin_ROOT_Cpp}
	${NFDataAgent_NosqlPlugin_ROOT_Hpp})
	
set_target_properties( NFDBLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFDBLogicPlugin")
set_target_properties( NFDBLogicPlugin PROPERTIES PREFIX "")
set_target_properties( NFDBLogicPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFDBLogicPlugin")

if(UNIX)
	target_link_libraries(NFDBLogicPlugin -static-libstdc++)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
else()
	target_link_libraries(NFDBLogicPlugin -static-libstdc++)
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()