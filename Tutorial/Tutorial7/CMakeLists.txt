set(ProjectName "Tutorial7")
file(GLOB Tutorial7_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial7_ROOT_Hpp 
	*.h)

add_library(Tutorial7 STATIC
	${Tutorial7_ROOT_Cpp}
	${Tutorial7_ROOT_Hpp})
	
set_target_properties( Tutorial7 PROPERTIES OUTPUT_NAME_DEBUG "Tutorial7_d")
set_target_properties( Tutorial7 PROPERTIES PREFIX "")
set_target_properties( Tutorial7 PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("Tutorial7")

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
