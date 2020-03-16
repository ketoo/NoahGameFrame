set(ProjectName "NFServer")

file(GLOB NFServer_ROOT_Cpp *.cpp)
file(GLOB NFServer_ROOT_Hpp *.h)

add_executable(NFServer ${NFServer_ROOT_Cpp} ${NFServer_ROOT_Hpp})

#link_NFSDK("NFServer")


target_link_libraries(NFServer NFPluginLoader)


if (UNIX)
	add_dependencies(NFServer NFCore)

	#if (CMAKE_BUILD_TYPE MATCHES "Release")
	#	target_link_libraries(NFServer libSDL2.a)
	#else()
	#	target_link_libraries(NFServer libSDL2d.a)
	#endif()

	target_link_libraries(NFServer -static-libstdc++ -pthread)

else()

endif()

set_target_properties( NFServer PROPERTIES OUTPUT_NAME_DEBUG "NFServer")
set_target_properties( NFServer PROPERTIES
	FOLDER "NFComm/NFServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )


add_definitions(-D_X64 -D_CONSOLE)
#add_definitions(-D_X64 -D_CONSOLE -DNF_DYNAMIC_PLUGIN)