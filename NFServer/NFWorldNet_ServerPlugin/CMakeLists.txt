file(GLOB NFWorldNet_ServerPlugin_ROOT_Cpp
	*.cpp)
file(GLOB NFWorldNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFWorldNet_ServerPlugin SHARED
	${NFWorldNet_ServerPlugin_ROOT_Cpp}
	${NFWorldNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFWorldNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFWorldNet_ServerPlugin_d")
set_target_properties( NFWorldNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/WorldServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK(NFWorldNet_ServerPlugin)

add_definitions(
	-DWIN
	-D_USRDLL
	-DELPP_NO_DEFAULT_LOG_FILE
)
