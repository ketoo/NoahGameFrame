set(ProjectName "NFMasterServerPlugin")
file(GLOB NFMasterServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFMasterServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFMasterServerPlugin SHARED
	${NFMasterServerPlugin_ROOT_Cpp}
	${NFMasterServerPlugin_ROOT_Hpp})
	
set_target_properties( NFMasterServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFMasterServerPlugin_d")
set_target_properties( NFMasterServerPlugin PROPERTIES 
	FOLDER "NFServer/MasterServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/ )
link_NFSDK("NFMasterServerPlugin")

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