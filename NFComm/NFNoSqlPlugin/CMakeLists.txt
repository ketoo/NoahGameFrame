set(ProjectName "NFNoSqlPlugin")

file(GLOB NFNoSqlPlugin_ROOT_Cpp *.cpp)
file(GLOB NFNoSqlPlugin_ROOT_CC *.cc)
file(GLOB NFNoSqlPlugin_ROOT_Hpp *.h)
	
if(UNIX)
	add_library(NFNoSqlPlugin SHARED
		${NFNoSqlPlugin_ROOT_Cpp}
		${NFNoSqlPlugin_ROOT_CC}
		${NFNoSqlPlugin_ROOT_Hpp}
		${SolutionDir}/Dependencies/redis-cplusplus-client/anet.c
		${SolutionDir}/Dependencies/redis-cplusplus-client/anet.h)
	target_include_directories(NFNoSqlPlugin PUBLIC
		${SolutionDir}/Dependencies/redis-cplusplus-client/)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFNoSqlPlugin libtheron.a)
	else()
		target_link_libraries(NFNoSqlPlugin libtherond.a)
	endif()
	add_definitions(
			-D_USRDLL
			-DTHERON_CPP11
		)
else()
	add_library(NFNoSqlPlugin SHARED
		${NFNoSqlPlugin_ROOT_Cpp}
		${NFNoSqlPlugin_ROOT_CC}
		${NFNoSqlPlugin_ROOT_Hpp})
	target_include_directories(NFNoSqlPlugin PUBLIC
		${SolutionDir}/Dependencies/redis-cplusplus-client/msvc/anet_win32/)
	add_dependencies(NFNoSqlPlugin libprotobuf NFMessageDefine)
	target_link_libraries(NFNoSqlPlugin anet_win64 libprotobuf Theron NFMessageDefine)
	add_definitions(
			-D_USRDLL
			-DTHERON_CPP11
		)
endif()
link_NFSDK("NFNoSqlPlugin")
set_target_properties( NFNoSqlPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFNoSqlPlugin_d")
set_target_properties( NFNoSqlPlugin PROPERTIES PREFIX "")
set_target_properties( NFNoSqlPlugin PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )