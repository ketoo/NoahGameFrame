set(ProjectName "NFDBNet_ClientPlugin")
file(GLOB NFDBNet_ClientPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFDBNet_ClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFDBNet_ClientPlugin SHARED
	${NFDBNet_ClientPlugin_ROOT_Cpp}
	${NFDBNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFDBNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFDBNet_ClientPlugin_d")
set_target_properties( NFDBNet_ClientPlugin PROPERTIES PREFIX "")
set_target_properties( NFDBNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/WorldServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFDBNet_ClientPlugin")

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
