set(ProjectName "NFConfigPlugin")
file(GLOB NFConfigPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFConfigPlugin_ROOT_Hpp 
	*.h)
add_library(NFConfigPlugin STATIC ${NFConfigPlugin_ROOT_Cpp} ${NFConfigPlugin_ROOT_Hpp})

set_target_properties( NFConfigPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFConfigPlugin")
set_target_properties( NFConfigPlugin PROPERTIES PREFIX "")
set_target_properties( NFConfigPlugin PROPERTIES 
	FOLDER "NFComm/NFConfigPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

if(UNIX)
	add_dependencies(NFConfigPlugin NFCore)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
	else()
	endif()

	target_link_libraries(NFConfigPlugin  NFCore)
	target_link_libraries(NFConfigPlugin -static-libstdc++)
else()
	message("${ProjectName} ${CMAKE_BUILD_TYPE}")
	
	target_link_libraries(NFConfigPlugin  NFCore)
	target_link_libraries(NFConfigPlugin -static-libstdc++)
endif()
