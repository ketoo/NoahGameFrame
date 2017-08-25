file(GLOB navigation_ROOT_Cpp
	*.cpp)
file(GLOB navigation_ROOT_Hpp
	*.hpp)
file(GLOB navigation_ROOT_H
	*.h)

add_library(navigation ${navigation_ROOT_Cpp} ${navigation_ROOT_H} ${navigation_ROOT_Hpp})
add_definitions(-DNF_NONCLIENT_BUILD)
set_target_properties( navigation PROPERTIES OUTPUT_NAME_DEBUG "navigation_d")

if( WIN32 )
    set_target_properties( navigation PROPERTIES PREFIX "")
endif()

set_target_properties( navigation PROPERTIES 
	FOLDER "NFDeps"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
	
if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(navigation )
	else()
		target_link_libraries(navigation )
	endif()
else()
	target_link_libraries(navigation )
endif()

