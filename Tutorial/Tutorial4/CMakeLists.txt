set(ProjectName "Tutorial4Plugin")
file(GLOB Tutorial4_ROOT_Cpp
	*.cpp)
file(GLOB Tutorial4_ROOT_Hpp 
	*.h)

add_library(Tutorial4Plugin STATIC
	${Tutorial4_ROOT_Cpp}
	${Tutorial4_ROOT_Hpp})
	
set_target_properties( Tutorial4Plugin PROPERTIES OUTPUT_NAME_DEBUG "Tutorial4Plugin")
set_target_properties( Tutorial4Plugin PROPERTIES PREFIX "")
set_target_properties( Tutorial4Plugin PROPERTIES 
	FOLDER "Tutorial/Tutorial4"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("Tutorial4Plugin")

# NOTE: target_link_libraries 添加 PRIVATE；add_definitions 转 target_compile_definitions
if(UNIX)
	target_link_libraries(Tutorial4Plugin PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial4Plugin PRIVATE _USRDLL ELPP_NO_DEFAULT_LOG_FILE)

else()
	target_link_libraries(Tutorial4Plugin PRIVATE -static-libstdc++)
	target_compile_definitions(Tutorial4Plugin PRIVATE WIN _USRDLL ELPP_NO_DEFAULT_LOG_FILE)
endif()