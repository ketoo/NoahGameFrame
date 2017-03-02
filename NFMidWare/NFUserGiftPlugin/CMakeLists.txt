set(ProjectName "NFUserGiftPlugin")
file(GLOB NFUserGiftPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFUserGiftPlugin_ROOT_Hpp 
	*.h)

add_library(NFUserGiftPlugin SHARED
	${NFUserGiftPlugin_ROOT_Cpp}
	${NFUserGiftPlugin_ROOT_Hpp})
	
set_target_properties( NFUserGiftPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFUserGiftPlugin_d")
set_target_properties( NFUserGiftPlugin PROPERTIES PREFIX "")
set_target_properties( NFUserGiftPlugin PROPERTIES 
	FOLDER "NFMidWare/NFUserGiftPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFUserGiftPlugin")

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
