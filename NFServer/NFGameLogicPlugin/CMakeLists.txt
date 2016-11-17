set(ProjectName "NFGameLogicPlugin")
file(GLOB NFGameLogicPlugin_ROOT_Cpp
	*.cpp)

file(GLOB NFGameLogicPlugin_ROOT_Hpp 
	*.h)
	
remove_file(NFGameLogicPlugin_ROOT_Cpp "NFCRebornItemConsumeProcessModule.cpp")
remove_file(NFGameLogicPlugin_ROOT_Hpp "NFCRebornItemConsumeProcessModule.h")

remove_file(NFGameLogicPlugin_ROOT_Cpp "NFCNormalConsumeProcessModule.cpp")
remove_file(NFGameLogicPlugin_ROOT_Hpp "NFCNormalConsumeProcessModule.h")

remove_file(NFGameLogicPlugin_ROOT_Cpp "NFCBriefSkillConsumeProcessModule.cpp")
remove_file(NFGameLogicPlugin_ROOT_Hpp "NFCBriefSkillConsumeProcessModule.h")

remove_file(NFGameLogicPlugin_ROOT_Cpp "NFCBulletSkillConsumeProcessModule.cpp")
remove_file(NFGameLogicPlugin_ROOT_Hpp "NFCBulletSkillConsumeProcessModule.h")

remove_file(NFGameLogicPlugin_ROOT_Cpp "NFCHeroItemConsumeProcessModule.cpp")
remove_file(NFGameLogicPlugin_ROOT_Hpp "NFCHeroItemConsumeProcessModule.h")

remove_file(NFGameLogicPlugin_ROOT_Cpp "NFCPotionItemConsumeProcessModule.cpp")
remove_file(NFGameLogicPlugin_ROOT_Hpp "NFCPotionItemConsumeProcessModule.h")

remove_file(NFGameLogicPlugin_ROOT_Cpp "NFCItemConsumeManagerModule.cpp")
remove_file(NFGameLogicPlugin_ROOT_Hpp "NFCItemConsumeManagerModule.h")


add_library(NFGameLogicPlugin SHARED
	${NFGameLogicPlugin_ROOT_Cpp}
	${NFGameLogicPlugin_ROOT_Hpp})
	
set_target_properties( NFGameLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGameLogicPlugin_d")
set_target_properties( NFGameLogicPlugin PROPERTIES PREFIX "")
set_target_properties( NFGameLogicPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
link_NFSDK("NFGameLogicPlugin")

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