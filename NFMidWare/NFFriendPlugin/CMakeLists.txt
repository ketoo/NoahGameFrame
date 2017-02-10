set(ProjectName "NFFriendPlugin")
file(GLOB NFFriendPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFFriendPlugin_ROOT_Hpp 
	*.h)

add_library(NFFriendPlugin SHARED
	${NFFriendPlugin_ROOT_Cpp}
	${NFFriendPlugin_ROOT_Hpp})
	
set_target_properties( NFFriendPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFFriendPlugin_d")
set_target_properties( NFFriendPlugin PROPERTIES PREFIX "")
set_target_properties( NFFriendPlugin PROPERTIES 
	FOLDER "NFMidWare/NFFriendPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFFriendPlugin")

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
