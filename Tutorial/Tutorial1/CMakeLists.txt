set(ProjectName "Tutorial1")
file(GLOB Tutorial1_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial1_ROOT_Hpp 
	*.h)

add_library(Tutorial1 SHARED
	${Tutorial1_ROOT_Cpp}
	${Tutorial1_ROOT_Hpp})
	
set_target_properties( Tutorial1 PROPERTIES OUTPUT_NAME_DEBUG "Tutorial1_d")
set_target_properties( Tutorial1 PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("Tutorial1")

if(UNIX)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
else()
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName).dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName).pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName).pdb")
	else()
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.dll" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName)_d.dll")
		add_custom_command(TARGET ${ProjectName}
							POST_BUILD
							COMMAND copy /y "$(SolutionDir)\\..\\_Out\\Comm\\$(Configuration)\\$(ProjectName)_d.pdb" "$(SolutionDir)\\..\\_Out\\Server\\$(Configuration)\\Tutorial1\\$(ProjectName)_d.pdb")
	endif()
endif()
