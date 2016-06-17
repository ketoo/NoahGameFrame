file(GLOB Tutorial2_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial2_ROOT_Hpp 
	*.h)

add_library(Tutorial2 SHARED
	${Tutorial2_ROOT_Cpp}
	${Tutorial2_ROOT_Hpp})
	
set_target_properties( Tutorial2 PROPERTIES OUTPUT_NAME_DEBUG "Tutorial2_d")
set_target_properties( Tutorial2 PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("Tutorial2")

add_definitions(
	-DWIN
	-D_USRDLL
	-DELPP_NO_DEFAULT_LOG_FILE
)