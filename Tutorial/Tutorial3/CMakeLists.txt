file(GLOB Tutorial3Plugin_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial3Plugin_ROOT_Hpp 
	*.h)

add_library(Tutorial3Plugin SHARED
	${Tutorial3Plugin_ROOT_Cpp}
	${Tutorial3Plugin_ROOT_Hpp})
	
set_target_properties( Tutorial3Plugin PROPERTIES OUTPUT_NAME_DEBUG "Tutorial3Plugin_d")
set_target_properties( Tutorial3Plugin PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("Tutorial3Plugin")

add_definitions(
	-DWIN
	-D_USRDLL
	-DELPP_NO_DEFAULT_LOG_FILE
)