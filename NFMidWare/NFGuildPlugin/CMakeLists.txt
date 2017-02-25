set(ProjectName "NFGuildPlugin")
file(GLOB NFGuildPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFGuildPlugin_ROOT_Hpp 
	*.h)

remove_file(NFGuildPlugin_ROOT_Cpp "NFCGuildEctypeModule.cpp")
remove_file(NFGuildPlugin_ROOT_Hpp "NFCGuildEctypeModule.h")

add_library(NFGuildPlugin SHARED
	${NFGuildPlugin_ROOT_Cpp}
	${NFGuildPlugin_ROOT_Hpp})
	
set_target_properties( NFGuildPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGuildPlugin_d")
set_target_properties( NFGuildPlugin PROPERTIES PREFIX "")
set_target_properties( NFGuildPlugin PROPERTIES 
	FOLDER "NFMidWare/NFGuildPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFGuildPlugin")

if(UNIX)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	
else()
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
endif()
