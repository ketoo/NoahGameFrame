set(ProjectName "NFAIServerNet_ServerPlugin")
file(GLOB NFAIServerNet_ServerPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFAIServerNet_ServerPlugin_ROOT_Hpp 
	*.h)

add_library(NFAIServerNet_ServerPlugin SHARED
	${NFAIServerNet_ServerPlugin_ROOT_Cpp}
	${NFAIServerNet_ServerPlugin_ROOT_Hpp})
	
set_target_properties( NFAIServerNet_ServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFAIServerNet_ServerPlugin_d")
set_target_properties( NFAIServerNet_ServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFAIServerNet_ServerPlugin PROPERTIES 
	FOLDER "NFServer/AIServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFAIServerNet_ServerPlugin")

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
endif()
