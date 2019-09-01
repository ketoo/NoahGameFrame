set(ProjectName "NFTeamPlugin")
file(GLOB NFTeamPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFTeamPlugin_ROOT_Hpp 
	*.h)

add_library(NFTeamPlugin STATIC
	${NFTeamPlugin_ROOT_Cpp}
	${NFTeamPlugin_ROOT_Hpp})
	
set_target_properties( NFTeamPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFTeamPlugin_d")
set_target_properties( NFTeamPlugin PROPERTIES PREFIX "")
set_target_properties( NFTeamPlugin PROPERTIES 
	FOLDER "NFMidWare/NFTeamPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFTeamPlugin")

if(UNIX)

	target_link_libraries(NFTeamPlugin -static-libstdc++)

	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

	target_link_libraries(NFTeamPlugin -static-libstdc++)

	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
