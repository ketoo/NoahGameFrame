set(ProjectName "NFMysqlPlugin")
file(GLOB NFMysqlPlugin_ROOT_Cpp *.cpp)
file(GLOB NFMysqlPlugin_ROOT_CC *.cc)
file(GLOB NFMysqlPlugin_ROOT_Hpp *.h)
	
if(UNIX)
	add_library(NFMysqlPlugin SHARED
		${NFMysqlPlugin_ROOT_Cpp}
		${NFMysqlPlugin_ROOT_CC}
		${NFMysqlPlugin_ROOT_Hpp})
	target_include_directories(NFMysqlPlugin PUBLIC
		${SolutionDir}/Dependencies/mysql/
		${SolutionDir}/Dependencies/mysql/pp/)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFMysqlPlugin libmysqlclient.a)
		target_link_libraries(NFMysqlPlugin libmysqlpp.a)
	else()
		target_link_libraries(NFMysqlPlugin libmysqlclient.a)
		target_link_libraries(NFMysqlPlugin libmysqlpp.a)
	endif()
	add_definitions(
			-D_USRDLL
		)
else()
	add_library(NFMysqlPlugin SHARED
		${NFMysqlPlugin_ROOT_Cpp}
		${NFMysqlPlugin_ROOT_CC}
		${NFMysqlPlugin_ROOT_Hpp})
	target_include_directories(NFMysqlPlugin PUBLIC
		${SolutionDir}/Dependencies/mysqlpp/
		${SolutionDir}/Dependencies/mysql/)
	add_dependencies(NFMysqlPlugin libprotobuf NFMessageDefine)
	target_link_libraries(NFMysqlPlugin libprotobuf Theron NFMessageDefine)
	add_definitions(
			-D_USRDLL
			-DTHERON_CPP11
			-DWIN32_LEAN_AND_MEAN
		)
endif()
link_NFSDK("NFMysqlPlugin")
set_target_properties( NFMysqlPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFMysqlPlugin_d")
set_target_properties( NFMysqlPlugin PROPERTIES PREFIX "")
set_target_properties( NFMysqlPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )