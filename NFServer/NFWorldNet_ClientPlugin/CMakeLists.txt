set(ProjectName "NFWorldNet_ClientPlugin")
file(GLOB NFWorldNet_ClientPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFWorldNet_ClientPlugin_ROOT_Hpp 
	*.h)

add_library(NFWorldNet_ClientPlugin SHARED
	${NFWorldNet_ClientPlugin_ROOT_Cpp}
	${NFWorldNet_ClientPlugin_ROOT_Hpp})
	
set_target_properties( NFWorldNet_ClientPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFWorldNet_ClientPlugin_d")
set_target_properties( NFWorldNet_ClientPlugin PROPERTIES PREFIX "")
set_target_properties( NFWorldNet_ClientPlugin PROPERTIES 
	FOLDER "NFServer/WorldServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFWorldNet_ClientPlugin")

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
