
file(GLOB TEMP_SRC *.cpp)
add_library(NFConfigPlugin SHARED ${TEMP_SRC})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set_target_properties( NFConfigPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFConfigPlugin_d")
set_target_properties( NFConfigPlugin PROPERTIES 
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )

if(UNIX)
	add_dependencies(NFConfigPlugin NFCore)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFConfigPlugin libtheron.a NFCore)
	else()
		target_link_libraries(NFConfigPlugin libtherond.a NFCore)
	endif()
else()
	target_link_libraries(NFConfigPlugin anet_win64 libprotobuf lua Theron)
endif()
