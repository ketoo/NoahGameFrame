IF( WIN32 )
	message("win32_anet_win64")
	aux_source_directory(. DIR_SRCS)
	add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
	add_library(anet_win64 anet_win32.c)
	set_target_properties( anet_win64 PROPERTIES OUTPUT_NAME_DEBUG "anet_win64_d")
	set_target_properties( anet_win64 PROPERTIES PREFIX "")
	set_target_properties( anet_win64 PROPERTIES 
		FOLDER "NFDeps"
		ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
		RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
		LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
ELSE()
	message("not win32")
ENDIF()

