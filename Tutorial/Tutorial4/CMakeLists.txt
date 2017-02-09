set(ProjectName "Tutorial4Plugin")
file(GLOB Tutorial4_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial4_ROOT_Hpp 
	*.h)

add_library(Tutorial4Plugin SHARED
	${Tutorial4_ROOT_Cpp}
	${Tutorial4_ROOT_Hpp})
	
set_target_properties( Tutorial4Plugin PROPERTIES OUTPUT_NAME_DEBUG "Tutorial4Plugin_d")
set_target_properties( Tutorial4Plugin PROPERTIES PREFIX "")
set_target_properties( Tutorial4Plugin PROPERTIES 
	FOLDER "Tutorial"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("Tutorial4Plugin")

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