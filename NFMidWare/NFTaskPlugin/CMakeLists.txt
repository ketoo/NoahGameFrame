set(ProjectName "NFTaskPlugin")
file(GLOB NFTaskPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFTaskPlugin_ROOT_Hpp 
	*.h)

add_library(NFTaskPlugin SHARED
	${NFTaskPlugin_ROOT_Cpp}
	${NFTaskPlugin_ROOT_Hpp})
	
set_target_properties( NFTaskPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFTaskPlugin_d")
set_target_properties( NFTaskPlugin PROPERTIES PREFIX "")
set_target_properties( NFTaskPlugin PROPERTIES 
	FOLDER "NFMidWare/NFTaskPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFTaskPlugin")

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
