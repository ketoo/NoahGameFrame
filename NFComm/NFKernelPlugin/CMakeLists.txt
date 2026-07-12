set(ProjectName "NFKernelPlugin")

file(GLOB NFKernelPlugin_ROOT_Cpp *.cpp)
file(GLOB NFKernelPlugin_ROOT_Hpp *.h)

add_library(NFKernelPlugin STATIC
	${NFKernelPlugin_ROOT_Cpp}
	${NFKernelPlugin_ROOT_Hpp})

# NOTE: target_link_libraries 统一添加 PRIVATE 关键字
if(UNIX)
	add_dependencies(NFKernelPlugin NFCore NFConfigPlugin)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
	else()
	endif()
		
	target_link_libraries(NFKernelPlugin PRIVATE NFCore)
	target_link_libraries(NFKernelPlugin PRIVATE -static-libstdc++)
else()
	add_dependencies(NFKernelPlugin NFCore NFConfigPlugin)

	target_link_libraries(NFKernelPlugin PRIVATE -static-libstdc++)

endif()

set_target_properties( NFKernelPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFKernelPlugin")
set_target_properties( NFKernelPlugin PROPERTIES PREFIX "")
set_target_properties( NFKernelPlugin PROPERTIES
	FOLDER "NFComm/NFKernelPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

