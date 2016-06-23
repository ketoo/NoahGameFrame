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


add_library(NFGameLogicPlugin SHARED
	${NFGameLogicPlugin_ROOT_Cpp}
	${NFGameLogicPlugin_ROOT_Hpp})
	
set_target_properties( NFGameLogicPlugin PROPERTIES OUTPUT_NAME_DEBUG "NFGameLogicPlugin_d")
set_target_properties( NFGameLogicPlugin PROPERTIES 
	FOLDER "NFServer/GameServer"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/_Out/Comm/ )
link_NFSDK("NFGameLogicPlugin")

if(UNIX)
	add_definitions(
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_post_cmd_linux(${ProjectName} "NFLoginServer" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "NFWorldServer" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "NFMasterServer" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "NFProxyServer1" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "NFGameServer1" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial1" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial2" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial3" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial4" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial5" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial6" ".so" "" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial7" ".so" "" "lib")
	else()
		add_post_cmd_linux(${ProjectName} "NFLoginServer" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "NFWorldServer" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "NFMasterServer" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "NFProxyServer1" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "NFGameServer1" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial1" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial2" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial3" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial4" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial5" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial6" ".so" "_d" "lib")
		add_post_cmd_linux(${ProjectName} "Tutorial7" ".so" "_d" "lib")
	endif()
else()
	add_definitions(
		-DWIN
		-D_USRDLL
		-DELPP_NO_DEFAULT_LOG_FILE
	)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")		
		add_post_cmd_win(${ProjectName} "NFLoginServer" "dll" "")
		add_post_cmd_win(${ProjectName} "NFWorldServer" "dll" "")
		add_post_cmd_win(${ProjectName} "NFMasterServer" "dll" "")
		add_post_cmd_win(${ProjectName} "NFProxyServer1" "dll" "")
		add_post_cmd_win(${ProjectName} "NFGameServer1" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial1" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial2" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial3" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial4" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial5" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial6" "dll" "")
		add_post_cmd_win(${ProjectName} "Tutorial7" "dll" "")
	else()
		add_post_cmd_win(${ProjectName} "NFLoginServer" "dll" "_d")
		add_post_cmd_win(${ProjectName} "NFWorldServer" "dll" "_d")
		add_post_cmd_win(${ProjectName} "NFMasterServer" "dll" "_d")
		add_post_cmd_win(${ProjectName} "NFProxyServer1" "dll" "_d")
		add_post_cmd_win(${ProjectName} "NFGameServer1" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial1" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial2" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial3" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial4" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial5" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial6" "dll" "_d")
		add_post_cmd_win(${ProjectName} "Tutorial7" "dll" "_d")
	endif()
endif()