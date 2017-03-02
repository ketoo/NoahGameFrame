set(ProjectName "NFItemBagPlugin")
file(GLOB NFItemBagPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFItemBagPlugin_ROOT_Hpp 
	*.h)

add_library(NFItemBagPlugin SHARED
	${NFItemBagPlugin_ROOT_Cpp}
	${NFItemBagPlugin_ROOT_Hpp})
	
set_target_properties( NFItemBagPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFItemBagPlugin_d")
set_target_properties( NFItemBagPlugin PROPERTIES PREFIX "")
set_target_properties( NFItemBagPlugin PROPERTIES 
	FOLDER "NFMidWare/NFItemBagPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFItemBagPlugin")

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