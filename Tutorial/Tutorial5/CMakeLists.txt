file(GLOB Tutorial5Plugin_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial5Plugin_ROOT_Hpp 
	*.h)

add_library(Tutorial5Plugin SHARED
	${Tutorial5Plugin_ROOT_Cpp}
	${Tutorial5Plugin_ROOT_Hpp})
	
set_target_properties( Tutorial5Plugin PROPERTIES OUTPUT_NAME_DEBUG "Tutorial5Plugin_d")
set_target_properties( Tutorial5Plugin PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("Tutorial5Plugin")

add_definitions(
	-DWIN
	-D_USRDLL
	-DELPP_NO_DEFAULT_LOG_FILE
)