set(ProjectName "NFMessageDefine")
file(GLOB NFMessageDefine_ROOT_Cpp *.cpp)
file(GLOB NFMessageDefine_ROOT_CC *.cc)
file(GLOB NFMessageDefine_ROOT_Hpp *.h)

add_library(NFMessageDefine SHARED
	${NFMessageDefine_ROOT_Cpp}
	${NFMessageDefine_ROOT_CC}
	${NFMessageDefine_ROOT_Hpp})

if (APPLE)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFMessageDefine libprotobuf.a)
	else()
		target_link_libraries(NFMessageDefine libprotobuf.a)
	endif()
	add_definitions(
		-D_THREAD_SAFE
	)
elseif(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFMessageDefine )
	else()
		target_link_libraries(NFMessageDefine )
	endif()
	
else()
	add_dependencies(NFMessageDefine libprotobuf)
	target_link_libraries(NFMessageDefine  libprotobuf )
	add_definitions(
		-D_USRDLL
		-D_LIB
		-DLIBPROTOC_EXPORTS
		-DPROTOBUF_USE_DLLS
		-D_SCL_SECURE_NO_WARNINGS
		)

endif()
set_target_properties( NFMessageDefine PROPERTIES OUTPUT_NAME_DEBUG "NFMessageDefine_d")
set_target_properties( NFMessageDefine PROPERTIES PREFIX "")
set_target_properties( NFMessageDefine PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )