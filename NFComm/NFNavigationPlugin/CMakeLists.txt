set(ProjectName "NFNavigationPlugin")
file(GLOB NFNavigationPlugin_ROOT_Cpp *.cpp)
file(GLOB NFNavigationPlugin_ROOT_Hpp *.h)
	
if(UNIX)
	add_library(NFNavigationPlugin STATIC
		${NFNavigationPlugin_ROOT_Cpp}
		${NFNavigationPlugin_ROOT_Hpp})
	target_include_directories(NFNavigationPlugin PUBLIC
		${SolutionDir}/Dependencies/navigation/)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFNavigationPlugin navigation NFCore)
	else()
		target_link_libraries(NFNavigationPlugin navigation_d NFCore)
	endif()


	target_link_libraries(NFNavigationPlugin -static-libstdc++)

	add_definitions(
			-D_USRDLL
		)
else()
	add_library(NFNavigationPlugin STATIC
		${NFNavigationPlugin_ROOT_Cpp}
		${NFNavigationPlugin_ROOT_Hpp})
	target_include_directories(NFNavigationPlugin PUBLIC
		${SolutionDir}/Dependencies/navigation/)
	add_dependencies(NFNavigationPlugin navigation NFCore)
	target_link_libraries(NFNavigationPlugin navigation NFCore)
	target_link_libraries(NFNavigationPlugin -static-libstdc++)
	add_definitions(
			-D_USRDLL
			-DWIN32_LEAN_AND_MEAN
		)
endif()
link_NFSDK("NFNavigationPlugin")
set_target_properties( NFNavigationPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFNavigationPlugin")
set_target_properties( NFNavigationPlugin PROPERTIES PREFIX "")
set_target_properties( NFNavigationPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )