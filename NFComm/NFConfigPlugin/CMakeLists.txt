set(ProjectName "NFConfigPlugin")
file(GLOB TEMP_SRC *.cpp)
add_library(NFConfigPlugin SHARED ${TEMP_SRC})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set_target_properties( NFConfigPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFConfigPlugin_d")
set_target_properties( NFConfigPlugin PROPERTIES 
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )

if(UNIX)
	add_dependencies(NFConfigPlugin NFCore)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFConfigPlugin libtheron.a NFCore)
	else()
		target_link_libraries(NFConfigPlugin libtherond.a NFCore)
	endif()
else()
	message("${ProjectName} ${CMAKE_BUILD_TYPE}")
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFMasterServer\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFMasterServer\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFProxyServer1\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFProxyServer1\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial2\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial2\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial3\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial3\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial4\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial4\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial5\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial5\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial6\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial6\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial7\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial7\\$(ProjectName).pdb")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFMasterServer\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFMasterServer\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFProxyServer1\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFProxyServer1\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial2\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial2\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial3\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial3\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial4\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial4\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial5\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial5\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial6\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial6\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial7\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial7\\$(ProjectName)_d.pdb")
	endif()
	target_link_libraries(NFConfigPlugin anet_win64 libprotobuf lua Theron)
endif()
