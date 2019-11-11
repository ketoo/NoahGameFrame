set(ProjectName "NFNoSqlPlugin")

set(ProjectName "NFNoSqlPlugin")
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

if(UNIX)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFNoSqlPlugin hiredis)
	else()
		target_link_libraries(NFNoSqlPlugin hiredis)
	endif()

	target_link_libraries(NFNoSqlPlugin -static-libstdc++)

	add_definitions(
			-D_USRDLL
			-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFNoSqlPlugin hiredis)
	else()
		target_link_libraries(NFNoSqlPlugin hiredis_d)
	endif()

	target_link_libraries(NFNoSqlPlugin -static-libstdc++)
	add_definitions(
			-DWIN
			-D_USRDLL
			-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()

