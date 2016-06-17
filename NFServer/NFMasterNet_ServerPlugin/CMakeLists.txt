file(GLOB NFMasterNet_ServerPlugin_ROOT_Cpp
	*.cpp)
file(GLOB NFMasterNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFMasterNet_ServerPlugin SHARED
	${NFMasterNet_ServerPlugin_ROOT_Cpp}
	${NFMasterNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFMasterNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFMasterNet_ServerPlugin_d")
set_target_properties( NFMasterNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/MasterServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("NFMasterNet_ServerPlugin")

add_definitions(
	-DWIN
	-D_USRDLL
	-DELPP_NO_DEFAULT_LOG_FILE
)