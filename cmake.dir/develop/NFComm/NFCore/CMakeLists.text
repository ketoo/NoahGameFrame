file(GLOB NFCore_ROOT_Cpp
	*.cpp)

#Exclude this file
file(GLOB RemoveItems Example.cpp)
list(REMOVE_ITEM NFCore_ROOT_Cpp ${RemoveItems})

file(GLOB NFCore_ROOT_Hpp 
	*.h)
add_library(NFCore ${NFCore_ROOT_Cpp} ${NFCore_ROOT_Hpp})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set_target_properties( NFCore PROPERTIES OUTPUT_NAME_DEBUG "NFCore_d")
set_target_properties( NFCore PROPERTIES 
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
	
if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFCore )
	else()
		target_link_libraries(NFCore )
	endif()
else()
	target_link_libraries(NFCore )
endif()

