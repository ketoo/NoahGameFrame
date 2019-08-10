set(ProjectName "NFNetCli")

file(GLOB NFNet_ROOT_Cpp *.cpp)
file(GLOB NFNet_ROOT_Hpp *.h)

add_executable(NFNetCli ${NFNet_ROOT_Cpp} ${NFNet_ROOT_Hpp})

add_definitions(-DNF_NONCLIENT_BUILD)
set_target_properties( NFNetCli PROPERTIES OUTPUT_NAME_DEBUG "NFNetCli")
set_target_properties( NFNetCli PROPERTIES PREFIX "")
set_target_properties( NFNetCli PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir})

	add_dependencies(NFNetCli NFCore)
	
if (APPLE)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
	else()
	endif()

		target_link_libraries(NFNetCli NFCore)
		target_link_libraries(NFNetCli NFNetPlugin)
		target_link_libraries(NFNetCli libevent.a)
		target_link_libraries(NFNetCli libevent_core.a)
		target_link_libraries(NFNetCli libevent_extra.a)
		#target_link_libraries(NFNetCli libevent_openssl.a)
		target_link_libraries(NFNetCli libprotobuf.a)
		#target_link_libraries(NFNetCli libssl.a)
		#target_link_libraries(NFNetCli libcrypto.a)
		target_link_libraries(NFNetCli NFMessageDefine)

		target_link_libraries(NFNetCli -static-libstdc++)
	add_definitions(
		-D_THREAD_SAFE
	)
elseif(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
	else()
	endif()

		target_link_libraries(NFNetCli NFCore)
		target_link_libraries(NFNetCli NFNetPlugin)
		target_link_libraries(NFNetCli libevent.a)
		target_link_libraries(NFNetCli libevent_core.a)
		target_link_libraries(NFNetCli libevent_extra.a)
		#target_link_libraries(NFNetCli libevent_openssl.a)
		target_link_libraries(NFNetCli libprotobuf.a)
		#target_link_libraries(NFNetCli libssl.a)
		#target_link_libraries(NFNetCli libcrypto.a)
		target_link_libraries(NFNetCli NFMessageDefine)
		target_link_libraries(NFNetCli -static-libstdc++)
	add_definitions(
	)
else()
	target_link_libraries(NFNetSvr NFNetPlugin)
	target_link_libraries(NFNetCli  libprotobuf libevent_core)
	add_definitions("-D_LIB -DWIN32 -DWIN")
endif()