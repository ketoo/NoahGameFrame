set(ProjectName "NFNoSqlTestPlugin")
file(GLOB NFNoSqlTestPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFNoSqlTestPlugin_ROOT_HPP
	*.h)

add_library(NFNoSqlTestPlugin STATIC
	${NFNoSqlTestPlugin_ROOT_Cpp}
	${NFNoSqlTestPlugin_ROOT_HPP})
	
set_target_properties( NFNoSqlTestPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFNoSqlTestPlugin")
set_target_properties( NFNoSqlTestPlugin PROPERTIES PREFIX "")
set_target_properties( NFNoSqlTestPlugin PROPERTIES
	FOLDER "NFTestCases/NFNoSqlTestPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

link_NFSDK("NFNoSqlTestPlugin")

if(UNIX)
	target_link_libraries(NFNoSqlTestPlugin -static-libstdc++)

	add_definitions(
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

	target_link_libraries(NFNoSqlTestPlugin -static-libstdc++)
	
	add_definitions(
		-DWIN
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
