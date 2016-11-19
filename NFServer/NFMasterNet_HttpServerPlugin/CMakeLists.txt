set(ProjectName "NFMasterNet_HttpServerPlugin")
file(GLOB NFMasterNet_HttpServerPlugin_ROOT_Cpp *.cpp)
file(GLOB NFMasterNet_HttpServerPlugin_ROOT_Hpp *.h)

add_library(NFMasterNet_HttpServerPlugin SHARED
	${NFMasterNet_HttpServerPlugin_ROOT_Cpp}
	${NFMasterNet_HttpServerPlugin_ROOT_Hpp})
	
set_target_properties( NFMasterNet_HttpServerPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFMasterNet_HttpServerPlugin_d")
set_target_properties( NFMasterNet_HttpServerPlugin PROPERTIES PREFIX "")
set_target_properties( NFMasterNet_HttpServerPlugin PROPERTIES 
	FOLDER "NFServer/MasterServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFMasterNet_HttpServerPlugin")

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
