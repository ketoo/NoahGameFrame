
set(ProjectName "redisclient")
file(GLOB NFRedis_ROOT_C *.c)
file(GLOB NFRedis_ROOT_Hpp *.h)

add_library(redisclient
		${NFRedis_ROOT_C}
		${NFRedis_ROOT_Hpp})

set_target_properties( redisclient PROPERTIES OUTPUT_NAME_DEBUG "redisclient_d")
set_target_properties( redisclient PROPERTIES PREFIX "")
set_target_properties( redisclient PROPERTIES
		FOLDER "NFDeps"
		ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
		RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
		LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )