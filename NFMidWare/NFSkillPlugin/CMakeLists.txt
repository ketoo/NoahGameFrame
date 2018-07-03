set(ProjectName "NFSkillPlugin")
file(GLOB NFSkillPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFSkillPlugin_ROOT_Hpp 
	*.h)

add_library(NFSkillPlugin STATIC
	${NFSkillPlugin_ROOT_Cpp}
	${NFSkillPlugin_ROOT_Hpp})
	
set_target_properties( NFSkillPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFSkillPlugin_d")
set_target_properties( NFSkillPlugin PROPERTIES PREFIX "")
set_target_properties( NFSkillPlugin PROPERTIES 
	FOLDER "NFMidWare/NFSkillPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFSkillPlugin")

if(UNIX)

	target_link_libraries(NFSkillPlugin -static-libstdc++)

	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()
	target_link_libraries(NFSkillPlugin -static-libstdc++)

	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
