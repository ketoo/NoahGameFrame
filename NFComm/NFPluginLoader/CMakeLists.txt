set(ProjectName "NFPluginLoader")
file(GLOB NFPluginLoader_ROOT_Cpp *.cpp)
file(GLOB NFPluginLoader_ROOT_Hpp *.h)
#Exclude this file
file(GLOB RemoveItems_Cpp NFCPluginActor.cpp)
list(REMOVE_ITEM NFPluginLoader_ROOT_Cpp ${RemoveItems_Cpp})

file(GLOB RemoveItems_Hpp NFCPluginActor.h)
list(REMOVE_ITEM NFPluginLoader_ROOT_Hpp ${RemoveItems_Hpp})

add_executable(NFPluginLoader ${NFPluginLoader_ROOT_Cpp} ${NFPluginLoader_ROOT_Hpp})

if(UNIX)
	add_dependencies(NFPluginLoader NFCore)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		target_link_libraries(NFPluginLoader libtheron.a pthread dl)
	else()
		target_link_libraries(NFPluginLoader libtherond.a pthread dl)
	endif()
else()
	add_dependencies(NFPluginLoader NFCore Theron)
	target_link_libraries(NFPluginLoader Theron)
	
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFMasterServer\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFMasterServer\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFProxyServer1\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFProxyServer1\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial2\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial2\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial3\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial3\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial4\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial4\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial5\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial5\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial6\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial6\\$(ProjectName).pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial7\\$(ProjectName).exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial7\\$(ProjectName).pdb")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFMasterServer\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFMasterServer\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFProxyServer1\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFProxyServer1\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial2\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial2\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial3\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial3\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial4\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial4\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial5\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial5\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial6\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial6\\$(ProjectName)_d.pdb")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.exe" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial7\\$(ProjectName)_d.exe")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial7\\$(ProjectName)_d.pdb")
	endif()
endif()

set_target_properties( NFPluginLoader PROPERTIES OUTPUT_NAME_DEBUG "NFPluginLoader_d")
set_target_properties( NFPluginLoader PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )

add_definitions(-D_X64 -D_CONSOLE -DNF_DYNAMIC_PLUGIN)