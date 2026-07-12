set(ProjectName "NFServer")

file(GLOB NFServer_ROOT_Cpp *.cpp)
file(GLOB NFServer_ROOT_Hpp *.h)

add_executable(NFServer ${NFServer_ROOT_Cpp} ${NFServer_ROOT_Hpp})

#link_NFSDK("NFServer")


# NOTE: target_link_libraries 添加 PRIVATE
target_link_libraries(NFServer PRIVATE NFPluginLoader)


if (UNIX)
	add_dependencies(NFServer NFCore)

	#if (CMAKE_BUILD_TYPE MATCHES "Release")
	#	target_link_libraries(NFServer PRIVATE libSDL2.a)
	#else()
	#	target_link_libraries(NFServer PRIVATE libSDL2d.a)
	#endif()

	target_link_libraries(NFServer PRIVATE -static-libstdc++ -pthread)

else()

endif()

set_target_properties( NFServer PROPERTIES OUTPUT_NAME_DEBUG "NFServer")
set_target_properties( NFServer PROPERTIES
	FOLDER "NFComm/NFServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )


# NOTE: add_definitions 转 target_compile_definitions
target_compile_definitions(NFServer PRIVATE _X64 _CONSOLE)
#target_compile_definitions(NFServer PRIVATE _X64 _CONSOLE NF_DYNAMIC_PLUGIN)