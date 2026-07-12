set(ProjectName "Tutorial7")
file(GLOB Tutorial7_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial7_ROOT_Hpp 
	*.h)

add_library(Tutorial7 STATIC
	${Tutorial7_ROOT_Cpp}
	${Tutorial7_ROOT_Hpp})
	
set_target_properties( Tutorial7 PROPERTIES OUTPUT_NAME_DEBUG "Tutorial7")
set_target_properties( Tutorial7 PROPERTIES PREFIX "")
set_target_properties( Tutorial7 PROPERTIES 
	FOLDER "Tutorial/Tutorial7"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("Tutorial7")

# NOTE: target_link_libraries 添加 PRIVATE；add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(Tutorial7 PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial7 PRIVATE _USRDLL ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(Tutorial7 PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial7 PRIVATE WIN _USRDLL ELPP_NO_DEFAULT_LOG_FILE)

endif()

