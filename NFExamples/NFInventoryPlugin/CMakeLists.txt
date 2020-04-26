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

if(UNIX)

	target_link_libraries(NFInventoryPlugin -static-libstdc++)

	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()

	target_link_libraries(NFInventoryPlugin -static-libstdc++)

	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()