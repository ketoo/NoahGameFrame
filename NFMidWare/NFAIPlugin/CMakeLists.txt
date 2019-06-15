set(ProjectName "NFAIPlugin")
file(GLOB NFAIPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFAIPlugin_ROOT_Hpp 
	*.h)

add_library(NFAIPlugin STATIC
	${NFAIPlugin_ROOT_Cpp}
	${NFAIPlugin_ROOT_Hpp})
	
set_target_properties( NFAIPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFAIPlugin_d")
set_target_properties( NFAIPlugin PROPERTIES PREFIX "")
set_target_properties( NFAIPlugin PROPERTIES 
	FOLDER "NFMidWare/NFAIPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFAIPlugin")

if(UNIX)

	target_link_libraries(NFAIPlugin -static-libstdc++)

	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

	target_link_libraries(NFAIPlugin -static-libstdc++)

	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
