set(ProjectName "NFCoroutinePlugin")
file(GLOB NFCoroutinePlugin_NFCoroutineModule_Cpp *.cpp)
file(GLOB NFCoroutinePlugin_NFCoroutineModule_Hpp *.h)

add_library(NFCoroutinePlugin SHARED
	${NFCoroutinePlugin_NFCoroutineModule_Cpp} 
	${NFCoroutinePlugin_NFCoroutineModule_Hpp}) 

if(UNIX)
	add_dependencies(NFCoroutinePlugin NFCore)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFCoroutinePlugin  NFCore)
	else()
		target_link_libraries(NFCoroutinePlugin  NFCore)
	endif()

else()
	add_dependencies(NFCoroutinePlugin NFCore)

endif()
set_target_properties( NFCoroutinePlugin PROPERTIES OUTPUT_NAME_DEBUG "NFCoroutinePlugin_d")
set_target_properties( NFCoroutinePlugin PROPERTIES PREFIX "")
set_target_properties( NFCoroutinePlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )