set(ProjectName "NFNetSvr")

file(GLOB NFNet_ROOT_Cpp *.cpp)
file(GLOB NFNet_ROOT_Hpp *.h)

add_executable(NFNetSvr ${NFNet_ROOT_Cpp} ${NFNet_ROOT_Hpp})

add_definitions(-DNF_NONCLIENT_BUILD)
set_target_properties( NFNetSvr PROPERTIES OUTPUT_NAME_DEBUG "NFNetSvr")
set_target_properties( NFNetSvr PROPERTIES PREFIX "")
set_target_properties( NFNetSvr PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir})

	add_dependencies(NFNetSvr NFCore)
	
if (APPLE)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
	else()
	endif()
		target_link_libraries(NFNetSvr NFCore)
		target_link_libraries(NFNetSvr NFNetPlugin)
		target_link_libraries(NFNetSvr libevent.a)
		target_link_libraries(NFNetSvr libevent_core.a)
		target_link_libraries(NFNetSvr libevent_extra.a)
		#target_link_libraries(NFNetSvr libevent_openssl.a)
		target_link_libraries(NFNetSvr libprotobuf.a)
		#target_link_libraries(NFNetSvr libssl.a)
		#target_link_libraries(NFNetSvr libcrypto.a)
		target_link_libraries(NFNetSvr NFMessageDefine)

		target_link_libraries(NFNetSvr -static-libstdc++)
	add_definitions(
		-D_THREAD_SAFE
	)
elseif(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
	else()
	endif()

		target_link_libraries(NFNetSvr NFCore)
		target_link_libraries(NFNetSvr NFNetPlugin)
		target_link_libraries(NFNetSvr libevent.a)
		target_link_libraries(NFNetSvr libevent_core.a)
		target_link_libraries(NFNetSvr libevent_extra.a)
		#target_link_libraries(NFNetSvr libevent_openssl.a)
		target_link_libraries(NFNetSvr libprotobuf.a)
		#target_link_libraries(NFNetSvr libssl.a)
		#target_link_libraries(NFNetSvr libcrypto.a)
		target_link_libraries(NFNetSvr NFMessageDefine)
		target_link_libraries(NFNetSvr -static-libstdc++)
	add_definitions(
	)
else()
	target_link_libraries(NFNetSvr NFNetPlugin)
	target_link_libraries(NFNetSvr  libprotobuf libevent_core)
	add_definitions("-D_LIB -DWIN32 -DWIN")
endif()