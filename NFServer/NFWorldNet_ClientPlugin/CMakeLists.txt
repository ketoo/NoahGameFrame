set(ProjectName "NFWorldNet_ClientPlugin")
file(GLOB NFWorldNet_ClientPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFWorldNet_ClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFWorldNet_ClientPlugin SHARED
	${NFWorldNet_ClientPlugin_ROOT_Cpp}
	${NFWorldNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFWorldNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFWorldNet_ClientPlugin_d")
set_target_properties( NFWorldNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/WorldServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("NFWorldNet_ClientPlugin")

if(UNIX)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND cp -f "${PROJECT_SOURCE_DIR}/_Out/Comm/Release/lib${ProjectName}.so" "${PROJECT_SOURCE_DIR}/_Out/Server/Release/NFWorldServer/{ProjectName}.so")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND cp -f "${PROJECT_SOURCE_DIR}/_Out/Comm/${ProjectName}_d.so" "${PROJECT_SOURCE_DIR}/_Out/Server/Debug/NFWorldServer/{ProjectName}.so")
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
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName).pdb")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFWorldServer\\$(ProjectName)_d.pdb")
	endif()
endif()
