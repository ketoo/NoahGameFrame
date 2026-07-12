set(ProjectName "NFNavigationPlugin")
file(GLOB NFNavigationPlugin_ROOT_Cpp *.cpp)
file(GLOB NFNavigationPlugin_ROOT_Hpp *.h)

# NOTE: target_link_libraries 统一添加 PRIVATE；add_definitions 改为 target_compile_definitions
if(UNIX)
	add_library(NFNavigationPlugin STATIC
		${NFNavigationPlugin_ROOT_Cpp}
		${NFNavigationPlugin_ROOT_Hpp})
	target_include_directories(NFNavigationPlugin PUBLIC
		${SolutionDir}/Dependencies/navigation/)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFNavigationPlugin PRIVATE navigation NFCore)
	else()
		target_link_libraries(NFNavigationPlugin PRIVATE navigation NFCore)
	endif()


	target_link_libraries(NFNavigationPlugin PRIVATE -static-libstdc++)
else()
	add_library(NFNavigationPlugin STATIC
		${NFNavigationPlugin_ROOT_Cpp}
		${NFNavigationPlugin_ROOT_Hpp})
	target_include_directories(NFNavigationPlugin PUBLIC
		${SolutionDir}/Dependencies/navigation/)
	add_dependencies(NFNavigationPlugin navigation NFCore)
	target_link_libraries(NFNavigationPlugin PRIVATE navigation NFCore)
	target_link_libraries(NFNavigationPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFNavigationPlugin PRIVATE WIN32_LEAN_AND_MEAN)
endif()
link_NFSDK("NFNavigationPlugin")
set_target_properties( NFNavigationPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFNavigationPlugin")
set_target_properties( NFNavigationPlugin PROPERTIES PREFIX "")
set_target_properties( NFNavigationPlugin PROPERTIES
	FOLDER "NFComm/NFNavigationPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )