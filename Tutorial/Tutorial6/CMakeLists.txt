file(GLOB Tutorial6Plugin_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial6Plugin_ROOT_Hpp 
	*.h)

add_library(Tutorial6Plugin SHARED
	${Tutorial6Plugin_ROOT_Cpp}
	${Tutorial6Plugin_ROOT_Hpp})
	
set_target_properties( Tutorial6Plugin PROPERTIES OUTPUT_NAME_DEBUG "Tutorial6Plugin_d")
set_target_properties( Tutorial6Plugin PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("Tutorial6Plugin")

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