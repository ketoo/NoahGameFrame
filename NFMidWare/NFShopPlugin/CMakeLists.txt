set(ProjectName "NFShopPlugin")
file(GLOB NFShopPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFShopPlugin_ROOT_Hpp 
	*.h)

add_library(NFShopPlugin SHARED
	${NFShopPlugin_ROOT_Cpp}
	${NFShopPlugin_ROOT_Hpp})
	
set_target_properties( NFShopPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFShopPlugin_d")
set_target_properties( NFShopPlugin PROPERTIES PREFIX "")
set_target_properties( NFShopPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFShopPlugin")

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
