set(ProjectName "NFNetPlugin")

file(GLOB NFNet_ROOT_Cpp *.cpp)
file(GLOB NFNet_ROOT_Hpp *.h)

#Exclude this file
#file(GLOB RemoveItems_Cpp TestClient.cpp TestServer.cpp)
#list(REMOVE_ITEM NFNet_ROOT_Cpp ${RemoveItems_Cpp})

add_library(NFNetPlugin STATIC ${NFNet_ROOT_Cpp} ${NFNet_ROOT_Hpp})

# NOTE: 使用 target_compile_definitions 替代 add_definitions
target_compile_definitions(NFNetPlugin PRIVATE NF_NONCLIENT_BUILD)
set_target_properties( NFNetPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFNetPlugin")
set_target_properties( NFNetPlugin PROPERTIES PREFIX "")
set_target_properties( NFNetPlugin PROPERTIES
	FOLDER "NFComm/NFNetPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir})

	add_dependencies(NFNetPlugin NFCore)
	
if(UNIX)

		# NOTE: 统一使用 PRIVATE 关键字
		target_link_libraries(NFNetPlugin PRIVATE NFCore)
		target_link_libraries(NFNetPlugin PRIVATE libevent.a)
		target_link_libraries(NFNetPlugin PRIVATE libevent_core.a)
		target_link_libraries(NFNetPlugin PRIVATE libevent_extra.a)
		#target_link_libraries(NFNetPlugin PRIVATE libevent_openssl.a)
		#target_link_libraries(NFNetPlugin PRIVATE libssl.a)
		#target_link_libraries(NFNetPlugin PRIVATE libcrypto.a)
		target_link_libraries(NFNetPlugin PRIVATE NFMessageDefine)
		target_link_libraries(NFNetPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFNetPlugin PRIVATE _THREAD_SAFE)
else()



endif()