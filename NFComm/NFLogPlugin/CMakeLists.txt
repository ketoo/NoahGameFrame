file(GLOB NFLogPlugin_ROOT_Cpp *.cpp)
file(GLOB NFLogPlugin_ROOT_Hpp *.h)

add_library(NFLogPlugin SHARED
	${NFLogPlugin_ROOT_Cpp}
	${NFLogPlugin_ROOT_Hpp})

if(UNIX)
	add_dependencies(NFLogPlugin NFCore)
else()
	add_dependencies(NFLogPlugin NFCore Theron)
	target_link_libraries(NFLogPlugin anet_win64 libprotobuf lua Theron)
endif()

set_target_properties( NFLogPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLogPlugin_d")
set_target_properties( NFLogPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )

