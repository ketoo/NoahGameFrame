set(ProjectName "NFDataAgent_NosqlPlugin")
file(GLOB NFDataAgent_NosqlPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFDataAgent_NosqlPlugin_ROOT_Hpp 
	*.h)

add_library(NFDataAgent_NosqlPlugin SHARED
	${NFDataAgent_NosqlPlugin_ROOT_Cpp}
	${NFDataAgent_NosqlPlugin_ROOT_Hpp})
	
set_target_properties( NFDataAgent_NosqlPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFDataAgent_NosqlPlugin_d")
set_target_properties( NFDataAgent_NosqlPlugin PROPERTIES PREFIX "")
set_target_properties( NFDataAgent_NosqlPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFDataAgent_NosqlPlugin")

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