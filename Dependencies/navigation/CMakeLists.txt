file(GLOB navigation_ROOT_Cpp *.cpp)
file(GLOB navigation_ROOT_Hpp *.hpp)
file(GLOB navigation_ROOT_H *.h)

add_library(navigation STATIC
	${navigation_ROOT_Cpp} 
	${navigation_ROOT_H} 
		${navigation_ROOT_Hpp})

add_definitions(-DNF_NONCLIENT_BUILD)
set_target_properties( navigation PROPERTIES OUTPUT_NAME_DEBUG "navigation")

if( WIN32 )
    set_target_properties( navigation PROPERTIES PREFIX "")
endif()

set_target_properties( navigation PROPERTIES 
	FOLDER "NFDeps"
	ARCHIVE_OUTPUT_DIRECTORY ${DependenciesLibPath}
	RUNTIME_OUTPUT_DIRECTORY ${DependenciesLibPath}
	LIBRARY_OUTPUT_DIRECTORY ${DependenciesLibPath} )
	

target_link_libraries(navigation)
target_link_libraries(navigation -static-libstdc++)