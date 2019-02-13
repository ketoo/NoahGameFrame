set(ProjectName "NFScalePlugin")
file(GLOB NFScalePlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFScalePlugin_ROOT_Hpp 
	*.h)

add_library(NFScalePlugin STATIC
	${NFScalePlugin_ROOT_Cpp}
	${NFScalePlugin_ROOT_Hpp})
	
set_target_properties( NFScalePlugin PROPERTIES OUTPUT_NAME_DEBUG "NFScalePlugin_d")
set_target_properties( NFScalePlugin PROPERTIES PREFIX "")
set_target_properties( NFScalePlugin PROPERTIES 
	FOLDER "NFMidWare/NFScalePlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFScalePlugin")

if(UNIX)

	target_link_libraries(NFScalePlugin -static-libstdc++)

	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

	target_link_libraries(NFScalePlugin -static-libstdc++)

	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
