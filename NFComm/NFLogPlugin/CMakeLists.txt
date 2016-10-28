set(ProjectName "NFLogPlugin")
file(GLOB NFLogPlugin_ROOT_Cpp *.cpp)
file(GLOB NFLogPlugin_ROOT_Hpp *.h)

add_library(NFLogPlugin SHARED
	${NFLogPlugin_ROOT_Cpp}
	${NFLogPlugin_ROOT_Hpp})

if(UNIX)
	add_dependencies(NFLogPlugin NFCore)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFLogPlugin )
	else()
		target_link_libraries(NFLogPlugin )
	endif()
	

else()
	add_dependencies(NFLogPlugin NFCore )
	target_link_libraries(NFLogPlugin  libprotobuf )
	

endif()

set_target_properties( NFLogPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLogPlugin_d")
set_target_properties( NFLogPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Server/ )