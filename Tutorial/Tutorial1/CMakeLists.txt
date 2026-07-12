set(ProjectName "Tutorial1")
file(GLOB Tutorial1_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial1_ROOT_Hpp 
	*.h)

add_library(Tutorial1 STATIC
	${Tutorial1_ROOT_Cpp}
	${Tutorial1_ROOT_Hpp})
	
set_target_properties( Tutorial1 PROPERTIES OUTPUT_NAME_DEBUG "Tutorial1")
set_target_properties( Tutorial1 PROPERTIES PREFIX "")
set_target_properties( Tutorial1 PROPERTIES 
	FOLDER "Tutorial/Tutorial1"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("Tutorial1")

# NOTE: target_link_libraries 添加 PRIVATE；add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(Tutorial1 PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial1 PRIVATE _USRDLL ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(Tutorial1 PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial1 PRIVATE WIN _USRDLL ELPP_NO_DEFAULT_LOG_FILE)

endif()
