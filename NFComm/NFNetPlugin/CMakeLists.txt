set(ProjectName "NFNetPlugin")

file(GLOB NFNet_ROOT_Cpp *.cpp)
file(GLOB NFNet_ROOT_Hpp *.h)

#Exclude this file
#file(GLOB RemoveItems_Cpp TestClient.cpp TestServer.cpp)
#list(REMOVE_ITEM NFNet_ROOT_Cpp ${RemoveItems_Cpp})

add_library(NFNetPlugin STATIC ${NFNet_ROOT_Cpp} ${NFNet_ROOT_Hpp})

add_definitions(-DNF_NONCLIENT_BUILD)
set_target_properties( NFNetPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFNetPlugin")
set_target_properties( NFNetPlugin PROPERTIES PREFIX "")
set_target_properties( NFNetPlugin PROPERTIES
	FOLDER "NFComm/NFNetPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir})

	add_dependencies(NFNetPlugin NFCore)
	
if(UNIX)

		target_link_libraries(NFNetPlugin NFCore)
		target_link_libraries(NFNetPlugin libevent.a)
		target_link_libraries(NFNetPlugin libevent_core.a)
		target_link_libraries(NFNetPlugin libevent_extra.a)
		#target_link_libraries(NFNetPlugin libevent_openssl.a)
		#target_link_libraries(NFNetPlugin libssl.a)
		#target_link_libraries(NFNetPlugin libcrypto.a)
		target_link_libraries(NFNetPlugin NFMessageDefine)
		target_link_libraries(NFNetPlugin -static-libstdc++)
	add_definitions(
		-D_THREAD_SAFE
	)
else()



endif()