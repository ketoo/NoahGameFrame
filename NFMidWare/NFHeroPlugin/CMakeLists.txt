set(ProjectName "NFHeroPlugin")
file(GLOB NFHeroPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFHeroPlugin_ROOT_Hpp 
	*.h)

add_library(NFHeroPlugin SHARED
	${NFHeroPlugin_ROOT_Cpp}
	${NFHeroPlugin_ROOT_Hpp})
	
set_target_properties( NFHeroPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFHeroPlugin_d")
set_target_properties( NFHeroPlugin PROPERTIES PREFIX "")
set_target_properties( NFHeroPlugin PROPERTIES 
	FOLDER "NFMidWare/NFHeroPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFHeroPlugin")

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
