set(ProjectName "NFPluginLoader")
file(GLOB NFPluginLoader_ROOT_Cpp *.cpp)
file(GLOB NFPluginLoader_ROOT_Hpp *.h)

add_library(NFPluginLoader STATIC
		${NFPluginLoader_ROOT_Cpp}
		${NFPluginLoader_ROOT_Hpp})

# NOTE: 所有 target_link_libraries 统一为 PRIVATE 关键字，避免与其它 keyword 签名混用报错
target_link_libraries(NFPluginLoader PRIVATE NFCore)
target_link_libraries(NFPluginLoader PRIVATE NFMessageDefine)
target_link_libraries(NFPluginLoader PRIVATE NFActorPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFConfigPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFKernelPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFLogPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFLuaScriptPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFNavigationPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFNetPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFNoSqlPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFSecurityPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFTestPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFChatPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFConsumeManagerPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFInventoryPlugin)

#target_link_libraries(NFPluginLoader PRIVATE NFRenderPlugin)
#target_link_libraries(NFPluginLoader PRIVATE NFBluePrintPlugin)

target_link_libraries(NFPluginLoader PRIVATE NFDBLogicPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFDBNet_ClientPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFDBNet_ServerPlugin)

target_link_libraries(NFPluginLoader PRIVATE NFGameServerPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFGameServerNet_ClientPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFGameServerNet_ServerPlugin)

target_link_libraries(NFPluginLoader PRIVATE NFLoginLogicPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFLoginNet_ClientPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFLoginNet_ServerPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFLoginNet_HttpServerPlugin)

target_link_libraries(NFPluginLoader PRIVATE NFMasterServerPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFMasterNet_ServerPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFMasterNet_HttpServerPlugin)

target_link_libraries(NFPluginLoader PRIVATE NFProxyLogicPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFProxyServerNet_ClientPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFProxyServerNet_ServerPlugin)

target_link_libraries(NFPluginLoader PRIVATE NFWorldNet_ClientPlugin)
target_link_libraries(NFPluginLoader PRIVATE NFWorldNet_ServerPlugin)

target_link_libraries(NFPluginLoader PRIVATE Tutorial1)
target_link_libraries(NFPluginLoader PRIVATE Tutorial2)
target_link_libraries(NFPluginLoader PRIVATE Tutorial3Plugin)
target_link_libraries(NFPluginLoader PRIVATE Tutorial4Plugin)
target_link_libraries(NFPluginLoader PRIVATE Tutorial5)
target_link_libraries(NFPluginLoader PRIVATE Tutorial6)
target_link_libraries(NFPluginLoader PRIVATE Tutorial7)

if (UNIX)
	target_link_libraries(NFPluginLoader PRIVATE -static-libstdc++ -pthread ${CMAKE_DL_LIBS})
else()
endif()

set_target_properties( NFPluginLoader PROPERTIES OUTPUT_NAME_DEBUG "NFPluginLoader")
set_target_properties( NFPluginLoader PROPERTIES PREFIX "")
set_target_properties( NFPluginLoader PROPERTIES
	FOLDER "NFComm/NFPluginLoader"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )

# NOTE: add_definitions 改为 target_compile_definitions
target_compile_definitions(NFPluginLoader PRIVATE _X64 _CONSOLE)