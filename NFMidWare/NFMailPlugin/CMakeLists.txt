set(ProjectName "NFMailPlugin")
file(GLOB NFMailPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFMailPlugin_ROOT_Hpp 
	*.h)

add_library(NFMailPlugin SHARED
	${NFMailPlugin_ROOT_Cpp}
	${NFMailPlugin_ROOT_Hpp})
	
set_target_properties( NFMailPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFMailPlugin_d")
set_target_properties( NFMailPlugin PROPERTIES PREFIX "")
set_target_properties( NFMailPlugin PROPERTIES 
	FOLDER "NFMidWare/NFMailPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFMailPlugin")

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
