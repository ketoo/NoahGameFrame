set(ProjectName "NFAOIPlugin")
file(GLOB NFAOIPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFAOIPlugin_ROOT_Hpp 
	*.h)

add_library(NFAOIPlugin STATIC
	${NFAOIPlugin_ROOT_Cpp}
	${NFAOIPlugin_ROOT_Hpp})
	
set_target_properties( NFAOIPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFAOIPlugin_d")
set_target_properties( NFAOIPlugin PROPERTIES PREFIX "")
set_target_properties( NFAOIPlugin PROPERTIES 
	FOLDER "NFMidWare/NFAOIPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFAOIPlugin")

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
