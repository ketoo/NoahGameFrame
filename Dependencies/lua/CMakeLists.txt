IF( WIN32 )
	message("win32_lua")
	aux_source_directory(. DIR_SRCS)

	add_library(lua ${DIR_SRCS})
	set_target_properties( lua PROPERTIES OUTPUT_NAME_DEBUG "lua_d")
	set_target_properties( lua PROPERTIES 
		FOLDER "NFDeps"
		ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
		LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
ELSE()
	message("not win32")
	aux_source_directory(. DIR_SRCS)

	add_library(lua ${DIR_SRCS})
	set_target_properties( lua PROPERTIES OUTPUT_NAME_DEBUG "lua_d")
	set_target_properties( lua PROPERTIES 
		FOLDER "NFDeps"
		ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
		LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
ENDIF()

