set(ProjectName "NFLoginLogicPlugin")
file(GLOB NFLoginLogicPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFLoginLogicPlugin_ROOT_Hpp 
	*.h)

add_library(NFLoginLogicPlugin SHARED
	${NFLoginLogicPlugin_ROOT_Cpp}
	${NFLoginLogicPlugin_ROOT_Hpp})
	
set_target_properties( NFLoginLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLoginLogicPlugin_d")
set_target_properties( NFLoginLogicPlugin PROPERTIES 
	FOLDER "NFServer/LoginServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/ )
link_NFSDK("NFLoginLogicPlugin")

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