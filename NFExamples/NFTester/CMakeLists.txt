set(ProjectName "NFTester")

file(GLOB NFTester_ROOT_Cpp *.cpp)
file(GLOB NFTester_ROOT_Hpp *.h)

add_executable(NFTester ${NFTester_ROOT_Cpp} ${NFTester_ROOT_Hpp})

#link_NFSDK("NFTester")


target_link_libraries(NFTester NFPluginLoader)


if (UNIX)
	add_dependencies(NFTester NFCore)

	#if (CMAKE_BUILD_TYPE MATCHES "Release")
	#	target_link_libraries(NFTester libSDL2.a)
	#else()
	#	target_link_libraries(NFTester libSDL2d.a)
	#endif()

	target_link_libraries(NFTester -static-libstdc++ -pthread)

else()

endif()

set_target_properties( NFTester PROPERTIES OUTPUT_NAME_DEBUG "NFTester")
set_target_properties( NFTester PROPERTIES
		FOLDER "NFComm/NFTester"
		ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
		RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
		LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )


add_definitions(-D_X64 -D_CONSOLE)
#add_definitions(-D_X64 -D_CONSOLE -DNF_DYNAMIC_PLUGIN)