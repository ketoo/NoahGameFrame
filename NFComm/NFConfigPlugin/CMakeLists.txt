
file(GLOB TEMP_SRC *.cpp)
add_library(NFConfigPlugin SHARED ${TEMP_SRC})
set_target_properties( NFConfigPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFConfigPlugin_d")
set_target_properties( NFConfigPlugin PROPERTIES 
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )

if(UNIX)
	target_link_libraries(NFConfigPlugin libtheron.a)
else()
	target_link_libraries(NFConfigPlugin anet_win64 libprotobuf lua Theron)
endif()
