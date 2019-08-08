set(ProjectName "NFPluginLoader")
file(GLOB NFPluginLoader_ROOT_Cpp *.cpp)
file(GLOB NFPluginLoader_ROOT_Hpp *.h)

add_executable(NFPluginLoader ${NFPluginLoader_ROOT_Cpp} ${NFPluginLoader_ROOT_Hpp})

#link_NFSDK("NFPluginLoader")



target_link_libraries(NFPluginLoader libprotobuf.a)
target_link_libraries(NFPluginLoader NFCore)
target_link_libraries(NFPluginLoader NFActorPlugin)
target_link_libraries(NFPluginLoader NFConfigPlugin)
target_link_libraries(NFPluginLoader NFKernelPlugin)
target_link_libraries(NFPluginLoader NFLogPlugin)
target_link_libraries(NFPluginLoader NFLuaScriptPlugin)
target_link_libraries(NFPluginLoader NFNavigationPlugin)
target_link_libraries(NFPluginLoader NFNetPlugin)
target_link_libraries(NFPluginLoader NFNoSqlPlugin)
target_link_libraries(NFPluginLoader NFScalePlugin)
target_link_libraries(NFPluginLoader NFSecurityPlugin)
target_link_libraries(NFPluginLoader NFTestPlugin)

target_link_libraries(NFPluginLoader NFDBLogicPlugin)
target_link_libraries(NFPluginLoader NFDBNet_ClientPlugin)
target_link_libraries(NFPluginLoader NFDBNet_ServerPlugin)

target_link_libraries(NFPluginLoader NFGameLogicPlugin)
target_link_libraries(NFPluginLoader NFGameServerPlugin)
target_link_libraries(NFPluginLoader NFGameServerNet_ClientPlugin)
target_link_libraries(NFPluginLoader NFGameServerNet_ServerPlugin)

target_link_libraries(NFPluginLoader NFLoginLogicPlugin)
target_link_libraries(NFPluginLoader NFLoginNet_ClientPlugin)
target_link_libraries(NFPluginLoader NFLoginNet_ServerPlugin)
target_link_libraries(NFPluginLoader NFLoginNet_HttpServerPlugin)

target_link_libraries(NFPluginLoader NFMasterServerPlugin)
target_link_libraries(NFPluginLoader NFMasterNet_ServerPlugin)
target_link_libraries(NFPluginLoader NFMasterNet_HttpServerPlugin)

target_link_libraries(NFPluginLoader NFProxyLogicPlugin)
target_link_libraries(NFPluginLoader NFProxyServerNet_ClientPlugin)
target_link_libraries(NFPluginLoader NFProxyServerNet_ServerPlugin)

target_link_libraries(NFPluginLoader NFWorldLogicPlugin)
target_link_libraries(NFPluginLoader NFWorldNet_ClientPlugin)
target_link_libraries(NFPluginLoader NFWorldNet_ServerPlugin)


target_link_libraries(NFPluginLoader NFChatPlugin)
target_link_libraries(NFPluginLoader NFFriendPlugin)
target_link_libraries(NFPluginLoader NFGuildPlugin)
target_link_libraries(NFPluginLoader NFHeroPlugin)
target_link_libraries(NFPluginLoader NFItemBagPlugin)
target_link_libraries(NFPluginLoader NFMailPlugin)
target_link_libraries(NFPluginLoader NFRankPlugin)
target_link_libraries(NFPluginLoader NFShopPlugin)
target_link_libraries(NFPluginLoader NFSkillPlugin)
target_link_libraries(NFPluginLoader NFTaskPlugin)
target_link_libraries(NFPluginLoader NFUserGiftPlugin)

target_link_libraries(NFPluginLoader Tutorial1)
target_link_libraries(NFPluginLoader Tutorial2)
target_link_libraries(NFPluginLoader Tutorial3Plugin)
target_link_libraries(NFPluginLoader Tutorial4Plugin)
target_link_libraries(NFPluginLoader Tutorial5)
target_link_libraries(NFPluginLoader Tutorial6)
target_link_libraries(NFPluginLoader Tutorial7)


if(UNIX)
	add_dependencies(NFPluginLoader NFCore)
	if (CMAKE_BUILD_TYPE MATCHES "Release")
	else()
	endif()

	target_link_libraries(NFPluginLoader )
	target_link_libraries(NFPluginLoader -static-libstdc++)

else()
	target_link_libraries(NFPluginLoader )
	
	target_link_libraries(NFPluginLoader -static-libstdc++)
endif()

target_link_libraries(NFPluginLoader ${CMAKE_DL_LIBS})
target_link_libraries(NFPluginLoader pthread)

set_target_properties( NFPluginLoader PROPERTIES OUTPUT_NAME_DEBUG "NFPluginLoader_d")
set_target_properties( NFPluginLoader PROPERTIES
	FOLDER "NFSDK"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )


add_definitions(-D_X64 -D_CONSOLE)
#add_definitions(-D_X64 -D_CONSOLE -DNF_DYNAMIC_PLUGIN)