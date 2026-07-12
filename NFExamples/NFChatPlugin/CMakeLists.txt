set(ProjectName "NFChatPlugin")
file(GLOB NFTutorialChatPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFTutorialChatPlugin_ROOT_Hpp 
	*.h)

add_library(NFChatPlugin STATIC
	${NFTutorialChatPlugin_ROOT_Cpp}
	${NFTutorialChatPlugin_ROOT_Hpp})
	
set_target_properties( NFChatPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFChatPlugin")
set_target_properties( NFChatPlugin PROPERTIES PREFIX "")
set_target_properties( NFChatPlugin PROPERTIES
	FOLDER "NFTutorial/NFChatPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
	
link_NFSDK("NFChatPlugin")

# NOTE: target_link_libraries 添加 PRIVATE；add_definitions 转 target_compile_definitions
if(UNIX)

	target_link_libraries(NFChatPlugin PRIVATE -static-libstdc++)

	target_compile_definitions(NFChatPlugin PRIVATE _USRDLL ELPP_NO_DEFAULT_LOG_FILE)
	
else()

	target_link_libraries(NFChatPlugin PRIVATE -static-libstdc++)

	target_compile_definitions(NFChatPlugin PRIVATE WIN _USRDLL ELPP_NO_DEFAULT_LOG_FILE)
endif()

