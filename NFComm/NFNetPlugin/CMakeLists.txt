set(ProjectName "NFNetPlugin")

file(GLOB NFNet_ROOT_Cpp *.cpp)
file(GLOB NFNet_ROOT_Hpp *.h)

#Exclude this file
file(GLOB RemoveItems_Cpp TestClient.cpp TestServer.cpp)
list(REMOVE_ITEM NFNet_ROOT_Cpp ${RemoveItems_Cpp})

add_library(NFNetPlugin SHARED ${NFNet_ROOT_Cpp} ${NFNet_ROOT_Hpp})
add_definitions(-DNF_NONCLIENT_BUILD)
set_target_properties( NFNetPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFNetPlugin_d")
set_target_properties( NFNetPlugin PROPERTIES PREFIX "")
set_target_properties( NFNetPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir})
if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFNetPlugin libevent.a)
		target_link_libraries(NFNetPlugin libevent_core.a)
		target_link_libraries(NFNetPlugin libprotobuf.so)
	else()
		target_link_libraries(NFNetPlugin libevent.a)
		target_link_libraries(NFNetPlugin libevent_core.a)
		target_link_libraries(NFNetPlugin libprotobuf.so)
	endif()
		add_definitions(
			-D_USRDLL
			-DTHERON_CPP11
		)
else()
	target_link_libraries(NFNetPlugin  libprotobuf libevent_core)
	add_definitions("-D_LIB -DWIN32 -DWIN")
endif()