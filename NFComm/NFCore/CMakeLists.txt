file(GLOB NFCore_ROOT_Cpp
	*.cpp)
file(GLOB NFCore_ROOT_Hpp
	*.hpp)
file(GLOB NFCore_ROOT_H
	*.h)

add_library(NFCore STATIC ${NFCore_ROOT_Cpp} ${NFCore_ROOT_H} ${NFCore_ROOT_Hpp})

add_definitions(-DNF_NONCLIENT_BUILD)
set_target_properties( NFCore PROPERTIES OUTPUT_NAME_DEBUG "NFCore")
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

	target_link_libraries(NFCore -static-libstdc++)
else()
	target_link_libraries(NFCore )
	target_link_libraries(NFCore -static-libstdc++)
endif()

