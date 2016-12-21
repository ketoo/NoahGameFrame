file(GLOB NFCore_ROOT_Cpp
	*.cpp)
file(GLOB NFCore_ROOT_Hpp
	*.hpp)
file(GLOB NFCore_ROOT_H
	*.h)

#Exclude this file
file(GLOB RemoveItems Example.cpp)
list(REMOVE_ITEM NFCore_ROOT_Cpp ${RemoveItems})

add_library(NFCore SHARED ${NFCore_ROOT_Cpp} ${NFCore_ROOT_H} ${NFCore_ROOT_Hpp})
add_definitions(-DNF_NONCLIENT_BUILD)
set_target_properties( NFCore PROPERTIES OUTPUT_NAME_DEBUG "NFCore_d")
set_target_properties( NFCore PROPERTIES PREFIX "")
set_target_properties( NFCore PROPERTIES 
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
	
if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFCore )
	else()
		target_link_libraries(NFCore )
	endif()
else()
	target_link_libraries(NFCore )
endif()

