set(ProjectName "NFLoginNet_ServerPlugin")
file(GLOB NFLoginNet_ServerPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFLoginNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFLoginNet_ServerPlugin SHARED
	${NFLoginNet_ServerPlugin_ROOT_Cpp}
	${NFLoginNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFLoginNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLoginNet_ServerPlugin_d")
set_target_properties( NFLoginNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/LoginServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("NFLoginNet_ServerPlugin")

if(UNIX)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND cp -f "${PROJECT_SOURCE_DIR}/_Out/Comm/lib${ProjectName}.so" "${PROJECT_SOURCE_DIR}/_Out/Server/Release/NFLoginServer/{ProjectName}.so")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND cp -f "${PROJECT_SOURCE_DIR}/_Out/Comm/lib${ProjectName}_d.so" "${PROJECT_SOURCE_DIR}/_Out/Server/Debug/NFLoginServer/{ProjectName}.so")
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
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName).pdb")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName)_d.pdb")
	endif()
endif()