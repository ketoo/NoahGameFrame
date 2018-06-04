set(ProjectName "NFTestPlugin")
file(GLOB NFTestPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFTestPlugin_ROOT_Hpp 
	*.h)

add_library(NFTestPlugin SHARED
	${NFTestPlugin_ROOT_Cpp}
	${NFTestPlugin_ROOT_Hpp})
	
set_target_properties( NFTestPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFTestPlugin_d")
set_target_properties( NFTestPlugin PROPERTIES PREFIX "")
set_target_properties( NFTestPlugin PROPERTIES 
	FOLDER "NFMidWare/NFTestPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFTestPlugin")

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
