set(ProjectName "NFKernelPlugin")
file(GLOB NFKernelPlugin_NFKernelModule_Cpp ./NFKernelModule/*.cpp)
file(GLOB NFKernelPlugin_NFKernelModule_Hpp ./NFKernelModule/*.h)
file(GLOB NFKernelPlugin_NFSceneModule_Cpp ./NFSceneModule/*.cpp)
file(GLOB NFKernelPlugin_NFSceneModule_Hpp ./NFSceneModule/*.h)
file(GLOB NFKernelPlugin_ROOT_Cpp *.cpp)
file(GLOB NFKernelPlugin_ROOT_Hpp *.h)

source_group("NFKernelModule" FILES ${NFKernelPlugin_NFKernelModule_Cpp})
source_group("NFSceneModule" FILES ${NFKernelPlugin_NFSceneModule_Cpp})
source_group("NFKernelModule" FILES ${NFKernelPlugin_NFKernelModule_Hpp})
source_group("NFSceneModule" FILES ${NFKernelPlugin_NFSceneModule_Hpp})

add_library(NFKernelPlugin STATIC
	${NFKernelPlugin_NFKernelModule_Cpp} 
	${NFKernelPlugin_NFSceneModule_Cpp} 
	${NFKernelPlugin_NFKernelModule_Hpp} 
	${NFKernelPlugin_NFSceneModule_Hpp}
	${NFKernelPlugin_ROOT_Cpp}
	${NFKernelPlugin_ROOT_Hpp})

if(UNIX)
	add_dependencies(NFKernelPlugin NFCore NFConfigPlugin)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
	else()
	endif()
		
	target_link_libraries(NFKernelPlugin  NFCore)
	target_link_libraries(NFKernelPlugin -static-libstdc++)
else()
	add_dependencies(NFKernelPlugin NFCore NFConfigPlugin)

	target_link_libraries(NFKernelPlugin -static-libstdc++)

endif()

set_target_properties( NFKernelPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFKernelPlugin")
set_target_properties( NFKernelPlugin PROPERTIES PREFIX "")
set_target_properties( NFKernelPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

