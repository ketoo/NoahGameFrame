set(ProjectName "NFRankPlugin")
file(GLOB NFRankPlugin_ROOT_Cpp
	*.cpp)
	
file(GLOB NFRankPlugin_ROOT_Hpp 
	*.h)

add_library(NFRankPlugin SHARED
	${NFRankPlugin_ROOT_Cpp}
	${NFRankPlugin_ROOT_Hpp})
	
set_target_properties( NFRankPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFRankPlugin_d")
set_target_properties( NFRankPlugin PROPERTIES PREFIX "")
set_target_properties( NFRankPlugin PROPERTIES 
	FOLDER "NFMidWare/NFRankPlugin"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFRankPlugin")

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
