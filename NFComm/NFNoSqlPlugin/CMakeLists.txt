set(ProjectName "NFNoSqlPlugin")

# NOTE: 移除重复的 set(ProjectName ...) 冗余声明
file(GLOB NFNoSqlPlugin_ROOT_Cpp
		*.cpp)

file(GLOB NFNoSqlPlugin_ROOT_Hpp
		*.h)

add_library(NFNoSqlPlugin STATIC
		${NFNoSqlPlugin_ROOT_Cpp}
		${NFNoSqlPlugin_Hpp})

set_target_properties( NFNoSqlPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFNoSqlPlugin")
set_target_properties( NFNoSqlPlugin PROPERTIES PREFIX "")
set_target_properties( NFNoSqlPlugin PROPERTIES
		FOLDER "NFComm/NFNoSqlPlugin"
		ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
		RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
		LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFNoSqlPlugin")

# NOTE: target_link_libraries 统一添加 PRIVATE 关键字；add_definitions 改为 target_compile_definitions
if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFNoSqlPlugin PRIVATE hiredis)
	else()
		target_link_libraries(NFNoSqlPlugin PRIVATE hiredis)
	endif()

	target_link_libraries(NFNoSqlPlugin PRIVATE -static-libstdc++)

	target_compile_definitions(NFNoSqlPlugin PRIVATE ELPP_NO_DEFAULT_LOG_FILE)

else()
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFNoSqlPlugin PRIVATE hiredis)
	else()
		target_link_libraries(NFNoSqlPlugin PRIVATE hiredis_d)
	endif()

	target_link_libraries(NFNoSqlPlugin PRIVATE -static-libstdc++)
	target_compile_definitions(NFNoSqlPlugin PRIVATE WIN ELPP_NO_DEFAULT_LOG_FILE)
endif()

