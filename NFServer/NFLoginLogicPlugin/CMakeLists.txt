set(ProjectName "NFLoginLogicPlugin")
file(GLOB NFLoginLogicPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFLoginLogicPlugin_ROOT_Hpp 
	*.h)

add_library(NFLoginLogicPlugin SHARED
	${NFLoginLogicPlugin_ROOT_Cpp}
	${NFLoginLogicPlugin_ROOT_Hpp})
	
set_target_properties( NFLoginLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLoginLogicPlugin_d")
set_target_properties( NFLoginLogicPlugin PROPERTIES 
	FOLDER "NFServer/LoginServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_Out/Comm/ )
link_NFSDK("NFLoginLogicPlugin")

if(UNIX)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND cp -f "${PROJECT_BINARY_DIR}/_Out/Comm/lib${ProjectName}.so" "${PROJECT_BINARY_DIR}/_Out/Server/Release/NFLoginServer/${ProjectName}.so")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND cp -f "${PROJECT_BINARY_DIR}/_Out/Comm/lib${ProjectName}_d.so" "${PROJECT_BINARY_DIR}/_Out/Server/Debug/NFLoginServer/${ProjectName}.so")
	endif()
else()
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	string(REGEX REPLACE "/" "\\\\" WIN32_PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR})

	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName).dll")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Server\\$(Configuration)\\NFLoginServer\\$(ProjectName)_d.pdb")
	endif()
endif()