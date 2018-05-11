set(ProjectName "NFRedisPlugin")

set(ProjectName "NFRedisPlugin")
file(GLOB NFRedisPlugin_ROOT_Cpp
		*.cpp)

file(GLOB NFRedisPlugin_ROOT_Hpp
		*.h)

add_library(NFRedisPlugin STATIC ${NFRedisPlugin_ROOT_Cpp} ${NFRedisPlugin_ROOT_Hpp})
		
#add_executable(NFRedisPlugin SHARED ${NFRedisPlugin_ROOT_Cpp} ${NFRedisPlugin_ROOT_Hpp})

set_target_properties( NFRedisPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFNoSqlPlugin_d")
set_target_properties( NFRedisPlugin PROPERTIES PREFIX "")
set_target_properties( NFRedisPlugin PROPERTIES
		FOLDER "NFComm/NFRedisPlugin"
		ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
		RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
		LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
		

# if(UNIX)
	# if (CMAKE_BUILD_TYPE MATCHES "Release")
		# target_link_libraries(NFRedisPlugin hiredis)
	# else()
		# target_link_libraries(NFRedisPlugin hiredis)
	# endif()

	# add_definitions(
			# -D_USRDLL
			# -DELPP_NO_DEFAULT_LOG_FILE
	# )

# else()
	# if (CMAKE_BUILD_TYPE MATCHES "Release")
		# target_link_libraries(NFRedisPlugin hiredis)
	# else()
		# target_link_libraries(NFRedisPlugin hiredis)
	# endif()

	# add_definitions(
			# -DWIN
			# -D_USRDLL
			# -DELPP_NO_DEFAULT_LOG_FILE
	# )
# endif()

