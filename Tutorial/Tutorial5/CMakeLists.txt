set(ProjectName "Tutorial5")
file(GLOB Tutorial5_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial5_ROOT_Hpp 
	*.h)

add_library(Tutorial5 STATIC
	${Tutorial5_ROOT_Cpp}
	${Tutorial5_ROOT_Hpp})
	
set_target_properties( Tutorial5 PROPERTIES OUTPUT_NAME_DEBUG "Tutorial5")
set_target_properties( Tutorial5 PROPERTIES PREFIX "")
set_target_properties( Tutorial5 PROPERTIES 
	FOLDER "Tutorial/Tutorial5"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("Tutorial5")

# NOTE: target_link_libraries 添加 PRIVATE；add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(Tutorial5 PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial5 PRIVATE _USRDLL ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(Tutorial5 PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial5 PRIVATE WIN _USRDLL ELPP_NO_DEFAULT_LOG_FILE)

endif()

