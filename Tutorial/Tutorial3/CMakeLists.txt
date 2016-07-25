set(ProjectName "Tutorial3Plugin")
file(GLOB Tutorial3_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial3_ROOT_Hpp 
	*.h)

add_library(Tutorial3Plugin SHARED
	${Tutorial3_ROOT_Cpp}
	${Tutorial3_ROOT_Hpp})
	
set_target_properties( Tutorial3Plugin PROPERTIES OUTPUT_NAME_DEBUG "Tutorial3Plugin_d")
set_target_properties( Tutorial3Plugin PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/_Out/Comm/ )
link_NFSDK("Tutorial3Plugin")

if(UNIX)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND cp -f "${PROJECT_BINARY_DIR}/_Out/Comm/lib${ProjectName}.so" "${PROJECT_BINARY_DIR}/_Out/Server/Release/Tutorial3/${ProjectName}.so")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND cp -f "${PROJECT_BINARY_DIR}/_Out/Comm/lib${ProjectName}_d.so" "${PROJECT_BINARY_DIR}/_Out/Server/Debug/Tutorial3/${ProjectName}_d.so")
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
							COMMAND copy /y "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Server\\$(Configuration)\\Tutorial3\\$(ProjectName).dll")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Server\\$(Configuration)\\Tutorial3\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "${WIN32_PROJECT_BINARY_DIR}\\_Out\\Server\\$(Configuration)\\Tutorial3\\$(ProjectName)_d.pdb")
	endif()
endif()