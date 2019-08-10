set(ProjectName "NFActorPlugin")
file(GLOB NFActorPlugin_ROOT_Cpp *.cpp)
file(GLOB NFActorPlugin_ROOT_CC *.cc)
file(GLOB NFActorPlugin_ROOT_Hpp *.h)

add_library(NFActorPlugin STATIC
	${NFActorPlugin_ROOT_Cpp}
	${NFActorPlugin_ROOT_CC}
	${NFActorPlugin_ROOT_Hpp})
	
if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFActorPlugin)
	else()
		target_link_libraries(NFActorPlugin)
	endif()

	target_link_libraries(NFActorPlugin -static-libstdc++)

	add_definitions(
		-D_USRDLL
		-D_LIB
		-DPROTOBUF_USE_DLLS
		-DNFDATABASEPLUGIN_EXPORTS
		-DWIN32_LEAN_AND_MEAN
		)
else()
	add_dependencies(NFActorPlugin libprotobuf NFMessageDefine)
	target_link_libraries(NFActorPlugin libprotobuf NFMessageDefine)
	target_link_libraries(NFActorPlugin -static-libstdc++)
	
	add_definitions(
		-D_USRDLL
		-D_LIB
		-DPROTOBUF_USE_DLLS
		-DNFDATABASEPLUGIN_EXPORTS
		-DWIN32_LEAN_AND_MEAN
		)
endif()
set_target_properties( NFActorPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFActorPlugin_d")
set_target_properties( NFActorPlugin PROPERTIES PREFIX "")
set_target_properties( NFActorPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )