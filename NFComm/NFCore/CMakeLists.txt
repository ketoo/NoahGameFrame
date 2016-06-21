file(GLOB TEMP_SRC *.cpp)
#Exclude this file
file(GLOB RemoveItems Example.cpp)
list(REMOVE_ITEM TEMP_SRC ${RemoveItems})
add_library(NFCore ${TEMP_SRC})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set_target_properties( NFCore PROPERTIES OUTPUT_NAME_DEBUG "NFCore_d")
set_target_properties( NFCore PROPERTIES 
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
	
if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFCore libtheron.a)
	else()
		target_link_libraries(NFCore libtherond.a)
	endif()
else()
	target_link_libraries(NFCore Theron)
endif()

