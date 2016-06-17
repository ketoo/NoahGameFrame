file(GLOB NFLoginNet_ServerPlugin_ROOT_Cpp
	*.cpp)
file(GLOB NFLoginNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFLoginNet_ServerPlugin SHARED
	${NFLoginNet_ServerPlugin_ROOT_Cpp}
	${NFLoginNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFLoginNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLoginNet_ServerPlugin_d")
set_target_properties( NFLoginNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/LoginServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("NFLoginNet_ServerPlugin")

add_definitions(
	-DWIN
	-D_USRDLL
	-DELPP_NO_DEFAULT_LOG_FILE
)