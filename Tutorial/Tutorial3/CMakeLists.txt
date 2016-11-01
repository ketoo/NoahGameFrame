set(ProjectName "Tutorial3Plugin")
file(GLOB Tutorial3_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial3_ROOT_Hpp 
	*.h)

add_library(Tutorial3Plugin SHARED
	${Tutorial3_ROOT_Cpp}
	${Tutorial3_ROOT_Hpp})
	
set_target_properties( Tutorial3Plugin PROPERTIES OUTPUT_NAME_DEBUG "Tutorial3Plugin_d")
set_target_properties( Tutorial3Plugin PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/ )
link_NFSDK("Tutorial3Plugin")

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