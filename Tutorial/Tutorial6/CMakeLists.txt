set(ProjectName "Tutorial6")
file(GLOB Tutorial6_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial6_ROOT_Hpp 
	*.h)

add_library(Tutorial6 SHARED
	${Tutorial6_ROOT_Cpp}
	${Tutorial6_ROOT_Hpp})
	
set_target_properties( Tutorial6 PROPERTIES OUTPUT_NAME_DEBUG "Tutorial6_d")
set_target_properties( Tutorial6 PROPERTIES PREFIX "")
set_target_properties( Tutorial6 PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("Tutorial6")

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
