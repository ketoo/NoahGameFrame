set(ProjectName "Tutorial2")
file(GLOB Tutorial2_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial2_ROOT_Hpp 
	*.h)

add_library(Tutorial2 SHARED
	${Tutorial2_ROOT_Cpp}
	${Tutorial2_ROOT_Hpp})
	
set_target_properties( Tutorial2 PROPERTIES OUTPUT_NAME_DEBUG "Tutorial2_d")
set_target_properties( Tutorial2 PROPERTIES PREFIX "")
set_target_properties( Tutorial2 PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("Tutorial2")

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
