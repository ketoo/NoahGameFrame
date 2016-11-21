set(ProjectName "NFConfigPlugin")
file(GLOB NFConfigPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFConfigPlugin_ROOT_Hpp 
	*.h)
add_library(NFConfigPlugin SHARED ${NFConfigPlugin_ROOT_Cpp} ${NFConfigPlugin_ROOT_Hpp})
set_target_properties( NFConfigPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFConfigPlugin_d")
set_target_properties( NFConfigPlugin PROPERTIES PREFIX "")
set_target_properties( NFConfigPlugin PROPERTIES 
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

if(UNIX)
	add_dependencies(NFConfigPlugin NFCore)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFConfigPlugin  NFCore)
	else()
		target_link_libraries(NFConfigPlugin  NFCore)
	endif()
else()
	message("${ProjectName} ${CMAKE_BUILD_TYPE}")
	target_link_libraries(NFConfigPlugin  libprotobuf )
endif()
