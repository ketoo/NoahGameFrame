set(ProjectName "NFLoginNet_ClientPlugin")
file(GLOB NFLoginNet_ClientPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFLoginNet_ClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFLoginNet_ClientPlugin SHARED
	${NFLoginNet_ClientPlugin_ROOT_Cpp}
	${NFLoginNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFLoginNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFLoginNet_ClientPlugin_d")
set_target_properties( NFLoginNet_ClientPlugin PROPERTIES PREFIX "")
set_target_properties( NFLoginNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/LoginServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFLoginNet_ClientPlugin")

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