set(ProjectName "NFGameServerNet_ClientPlugin")
file(GLOB NFGameServerNet_ClientPlugin_ROOT_Cpp
	dllmain.cpp
	NFCGameServerToWorldModule.cpp
	NFGameServerNet_ClientPlugin.cpp)
file(GLOB NFGameServerNet_ClientPlugin_ROOT_Hpp 
	NFCGameServerToWorldModule.h
	NFGameServerNet_ClientPlugin.h)

add_library(NFGameServerNet_ClientPlugin SHARED
	${NFGameServerNet_ClientPlugin_ROOT_Cpp}
	${NFGameServerNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFGameServerNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGameServerNet_ClientPlugin_d")
set_target_properties( NFGameServerNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )

link_NFSDK("NFGameServerNet_ClientPlugin")

if(UNIX)
	target_link_libraries(NFGameServerNet_ClientPlugin protobuf)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND cp -f "${PROJECT_SOURCE_DIR}/_Out/Comm/lib${ProjectName}.so" "${PROJECT_SOURCE_DIR}/_Out/Server/Release/NFGameServer1/{ProjectName}.so")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND cp -f "${PROJECT_SOURCE_DIR}/_Out/Comm/${ProjectName}_d.so" "${PROJECT_SOURCE_DIR}/_Out/Server/Debug/NFGameServer1/{ProjectName}.so")
	endif()
	
else()
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName).pdb")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFGameServer1\\$(ProjectName)_d.pdb")
	endif()
endif()