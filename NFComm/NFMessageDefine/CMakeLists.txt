set(ProjectName "NFMessageDefine")
file(GLOB NFMessageDefine_ROOT_Cpp *.cpp)
file(GLOB NFMessageDefine_ROOT_CC *.cc)
file(GLOB NFMessageDefine_ROOT_Hpp *.h)

add_library(NFMessageDefine STATIC
	${NFMessageDefine_ROOT_Cpp}
	${NFMessageDefine_ROOT_CC}
	${NFMessageDefine_ROOT_Hpp})

if (UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFMessageDefine libprotobuf.a)
	else()
		target_link_libraries(NFMessageDefine libprotobufd.a)
	endif()

	add_definitions(
		-D_THREAD_SAFE
	)

	target_link_libraries(NFMessageDefine -static-libstdc++)
else()


endif()

set_target_properties( NFMessageDefine PROPERTIES OUTPUT_NAME_DEBUG "NFMessageDefine")
set_target_properties( NFMessageDefine PROPERTIES PREFIX "")
set_target_properties( NFMessageDefine PROPERTIES
	FOLDER "NFComm/NFMessageDefine"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )