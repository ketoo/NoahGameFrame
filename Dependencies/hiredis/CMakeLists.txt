file(GLOB HIREDIS_ROOT_Cpp
	*.c)
file(GLOB HIREDIS_ROOT_Hpp
	*.hpp)
file(GLOB HIREDIS_ROOT_H
	*.h)

add_library(hiredis ${HIREDIS_ROOT_Cpp} ${HIREDIS_ROOT_Hpp} ${HIREDIS_ROOT_H})
add_definitions(-DNF_NONCLIENT_BUILD)
set_target_properties( hiredis PROPERTIES OUTPUT_NAME_DEBUG "hiredis_d")

set_target_properties( hiredis PROPERTIES
	FOLDER "NFDeps"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
	
if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(hiredis )
	else()
		target_link_libraries(hiredis )
	endif()
else()
	target_link_libraries(hiredis )
endif()

