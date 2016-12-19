set(ProjectName "NFGangPlugin")
file(GLOB NFGangPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFGangPlugin_ROOT_Hpp 
	*.h)

add_library(NFGangPlugin SHARED
	${NFGangPlugin_ROOT_Cpp}
	${NFGangPlugin_ROOT_Hpp})
	
set_target_properties( NFGangPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGangPlugin_d")
set_target_properties( NFGangPlugin PROPERTIES PREFIX "")
set_target_properties( NFGangPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFGangPlugin")

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
