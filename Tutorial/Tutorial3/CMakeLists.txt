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

# NOTE: target_link_libraries 添加 PRIVATE；add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(Tutorial3Plugin PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial3Plugin PRIVATE _USRDLL ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(Tutorial3Plugin PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial3Plugin PRIVATE WIN _USRDLL ELPP_NO_DEFAULT_LOG_FILE)
endif()