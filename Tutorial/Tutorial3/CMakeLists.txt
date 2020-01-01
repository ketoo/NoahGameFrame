set(ProjectName "Tutorial3Plugin")
file(GLOB Tutorial3_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial3_ROOT_Hpp 
	*.h)

add_library(Tutorial3Plugin STATIC
	${Tutorial3_ROOT_Cpp}
	${Tutorial3_ROOT_Hpp})
	
set_target_properties( Tutorial3Plugin PROPERTIES OUTPUT_NAME_DEBUG "Tutorial3Plugin")
set_target_properties( Tutorial3Plugin PROPERTIES PREFIX "")
set_target_properties( Tutorial3Plugin PROPERTIES 
	FOLDER "Tutorial/Tutorial3"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("Tutorial3Plugin")

if(UNIX)
	target_link_libraries(Tutorial3Plugin -static-libstdc++)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)

else()
	target_link_libraries(Tutorial3Plugin -static-libstdc++)
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()