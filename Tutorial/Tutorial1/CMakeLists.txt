set(ProjectName "Tutorial1")
file(GLOB Tutorial1_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial1_ROOT_Hpp 
	*.h)

add_library(Tutorial1 SHARED
	${Tutorial1_ROOT_Cpp}
	${Tutorial1_ROOT_Hpp})
	
set_target_properties( Tutorial1 PROPERTIES OUTPUT_NAME_DEBUG "Tutorial1_d")
set_target_properties( Tutorial1 PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/ )
link_NFSDK("Tutorial1")

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
