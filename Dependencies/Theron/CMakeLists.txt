IF( WIN32 )
	message("win32_theron")
	include_directories(Include)
	file(GLOB TEMP_SRC Theron/*.cpp)
	add_library(Theron ${TEMP_SRC})
	set_target_properties( Theron PROPERTIES OUTPUT_NAME_DEBUG "Theron_d")
	set_target_properties( Theron PROPERTIES PREFIX "")
	set_target_properties( Theron PROPERTIES
	FOLDER "NFDeps"
		ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
		RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
		LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
	add_definitions(-DTHERON_USE_STD_THREADS -D_X64)
ELSE()
	message("not win32")
ENDIF()