set(ProjectName "NFUIPlugin")
file(GLOB NFUIPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFUIPlugin_ROOT_Hpp 
	*.h)

add_library(NFUIPlugin STATIC
	${NFUIPlugin_ROOT_Cpp}
	${NFUIPlugin_ROOT_Hpp})
	
set_target_properties( NFUIPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFUIPlugin_d")
set_target_properties( NFUIPlugin PROPERTIES PREFIX "")
set_target_properties( NFUIPlugin PROPERTIES 
	FOLDER "NFMidWare/NFUIPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFUIPlugin")
if(UNIX)

	target_link_libraries(NFUIPlugin -static-libstdc++)

	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

	target_link_libraries(NFUIPlugin -static-libstdc++)
	
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
