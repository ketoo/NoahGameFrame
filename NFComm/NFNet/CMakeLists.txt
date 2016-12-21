file(GLOB NFNet_ROOT_Cpp *.cpp)
file(GLOB NFNet_ROOT_Hpp *.h)
#Exclude this file
file(GLOB RemoveItems_Cpp TestClient.cpp TestServer.cpp)
list(REMOVE_ITEM NFNet_ROOT_Cpp ${RemoveItems_Cpp})

add_library(NFNet SHARED ${NFNet_ROOT_Cpp} ${NFNet_ROOT_Hpp})
add_definitions(-DNF_NONCLIENT_BUILD)
set_target_properties( NFNet PROPERTIES OUTPUT_NAME_DEBUG "NFNet_d")
set_target_properties( NFNet PROPERTIES PREFIX "")
set_target_properties( NFNet PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir})
if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFNet )
	else()
		target_link_libraries(NFNet )
	endif()
else()
	target_link_libraries(NFNet  libprotobuf )
	add_definitions("-D_LIB -DWIN32 -DWIN")
endif()