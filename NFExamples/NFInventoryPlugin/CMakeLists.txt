set(ProjectName "NFInventoryPlugin")
file(GLOB NFTutorialInventoryPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFTutorialInventoryPlugin_ROOT_Hpp
	*.h)

add_library(NFInventoryPlugin STATIC
	${NFTutorialInventoryPlugin_ROOT_Cpp}
	${NFTutorialInventoryPlugin_ROOT_Hpp})
	
set_target_properties( NFInventoryPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFInventoryPlugin")
set_target_properties( NFInventoryPlugin PROPERTIES PREFIX "")
set_target_properties( NFInventoryPlugin PROPERTIES
	FOLDER "NFTutorial/NFInventoryPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFInventoryPlugin")

# NOTE: target_link_libraries 添加 PRIVATE；add_definitions 转 target_compile_definitions
if(UNIX)

	target_link_libraries(NFInventoryPlugin PRIVATE -static-libstdc++)

	target_compile_definitions(NFInventoryPlugin PRIVATE _USRDLL ELPP_NO_DEFAULT_LOG_FILE)
	
else()

	target_link_libraries(NFInventoryPlugin PRIVATE -static-libstdc++)

	target_compile_definitions(NFInventoryPlugin PRIVATE WIN _USRDLL ELPP_NO_DEFAULT_LOG_FILE)
endif()