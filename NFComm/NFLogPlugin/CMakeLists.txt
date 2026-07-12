set(ProjectName "NFLogPlugin")
file(GLOB NFLogPlugin_ROOT_Cpp *.cpp)
file(GLOB NFLogPlugin_ROOT_Hpp *.h)

add_library(NFLogPlugin STATIC
	${NFLogPlugin_ROOT_Cpp}
	${NFLogPlugin_ROOT_Hpp})

# NOTE: target_link_libraries 统一添加 PRIVATE 关键字
if(UNIX)

	if (CMAKE_BUILD_TYPE MATCHES "Release")
	else()
	endif()
	
	add_dependencies(NFLogPlugin NFCore )

	target_link_libraries(NFLogPlugin PRIVATE -static-libstdc++)
else()
	add_dependencies(NFLogPlugin NFCore )
	
	target_link_libraries(NFLogPlugin PRIVATE -static-libstdc++)
endif()

set_target_properties( NFLogPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLogPlugin")
set_target_properties( NFLogPlugin PROPERTIES PREFIX "")
set_target_properties( NFLogPlugin PROPERTIES
	FOLDER "NFComm/NFLogPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )