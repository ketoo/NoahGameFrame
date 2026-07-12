set(ProjectName "Tutorial2")
file(GLOB Tutorial2_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial2_ROOT_Hpp 
	*.h)

add_library(Tutorial2 STATIC
	${Tutorial2_ROOT_Cpp}
	${Tutorial2_ROOT_Hpp})
	
set_target_properties( Tutorial2 PROPERTIES OUTPUT_NAME_DEBUG "Tutorial2")
set_target_properties( Tutorial2 PROPERTIES PREFIX "")
set_target_properties( Tutorial2 PROPERTIES 
	FOLDER "Tutorial/Tutorial2"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("Tutorial2")

# NOTE: target_link_libraries 添加 PRIVATE；add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(Tutorial2 PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial2 PRIVATE _USRDLL ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(Tutorial2 PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial2 PRIVATE WIN _USRDLL ELPP_NO_DEFAULT_LOG_FILE)

endif()

