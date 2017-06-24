set(ProjectName "NFAIServerNet_ClientPlugin")
file(GLOB NFAIServerNet_ClientPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFAIServerNet_ClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFAIServerNet_ClientPlugin SHARED
	${NFAIServerNet_ClientPlugin_ROOT_Cpp}
	${NFAIServerNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFAIServerNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFAIServerNet_ClientPlugin_d")
set_target_properties( NFAIServerNet_ClientPlugin PROPERTIES PREFIX "")
set_target_properties( NFAIServerNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/AIServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

link_NFSDK("NFAIServerNet_ClientPlugin")

if(UNIX)
	target_link_libraries(NFAIServerNet_ClientPlugin protobuf)
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
	

endif()