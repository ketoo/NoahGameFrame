set(ProjectName "NFServer")
file(GLOB NFServer_ROOT_Cpp
	*.cpp)
	
file(GLOB NFServer_ROOT_Hpp 
	*.h)

add_library(NFServer STATIC
	${NFServer_ROOT_Cpp}
	${NFServer_ROOT_Hpp})
	
set_target_properties( NFServer PROPERTIES OUTPUT_NAME_DEBUG "NFServer")
set_target_properties( NFServer PROPERTIES PREFIX "")
set_target_properties( NFServer PROPERTIES 
	FOLDER "NFComm/NFServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFServer")

if(UNIX)

	target_link_libraries(NFServer -static-libstdc++)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

	target_link_libraries(NFServer -static-libstdc++)
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
