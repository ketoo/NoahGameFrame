set(ProjectName "NFPluginLoader")
file(GLOB NFPluginLoader_ROOT_Cpp *.cpp)
file(GLOB NFPluginLoader_ROOT_Hpp *.h)

add_library(NFPluginLoader STATIC
		${NFPluginLoader_ROOT_Cpp}
		${NFPluginLoader_ROOT_Hpp})


target_link_libraries(NFPluginLoader NFCore)
target_link_libraries(NFPluginLoader NFMessageDefine)
target_link_libraries(NFPluginLoader NFActorPlugin)
target_link_libraries(NFPluginLoader NFConfigPlugin)
target_link_libraries(NFPluginLoader NFKernelPlugin)
target_link_libraries(NFPluginLoader NFLogPlugin)
target_link_libraries(NFPluginLoader NFLuaScriptPlugin)
target_link_libraries(NFPluginLoader NFNavigationPlugin)
target_link_libraries(NFPluginLoader NFNetPlugin)
target_link_libraries(NFPluginLoader NFNoSqlPlugin)
target_link_libraries(NFPluginLoader NFSecurityPlugin)
target_link_libraries(NFPluginLoader NFTestPlugin)
target_link_libraries(NFPluginLoader NFChatPlugin)
target_link_libraries(NFPluginLoader NFConsumeManagerPlugin)
target_link_libraries(NFPluginLoader NFInventoryPlugin)

if (APPLE)
target_link_libraries(NFPluginLoader NFRenderPlugin)
target_link_libraries(NFPluginLoader NFBluePrintPlugin)
endif()

target_link_libraries(NFPluginLoader NFDBLogicPlugin)
target_link_libraries(NFPluginLoader NFDBNet_ClientPlugin)
target_link_libraries(NFPluginLoader NFDBNet_ServerPlugin)

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

target_link_libraries(NFPluginLoader NFWorldNet_ClientPlugin)
target_link_libraries(NFPluginLoader NFWorldNet_ServerPlugin)


#target_link_libraries(NFPluginLoader Tutorial1)
#target_link_libraries(NFPluginLoader Tutorial2)
#target_link_libraries(NFPluginLoader Tutorial3Plugin)
#target_link_libraries(NFPluginLoader Tutorial4Plugin)
#target_link_libraries(NFPluginLoader Tutorial5)
#target_link_libraries(NFPluginLoader Tutorial6)
#target_link_libraries(NFPluginLoader Tutorial7)


if (UNIX)
	add_dependencies(NFPluginLoader NFCore)

	#if (CMAKE_BUILD_TYPE MATCHES "Release")
	#	target_link_libraries(NFPluginLoader libSDL2.a)
	#else()
	#	target_link_libraries(NFPluginLoader libSDL2d.a)
	#endif()

	target_link_libraries(NFPluginLoader -static-libstdc++ -pthread ${CMAKE_DL_LIBS})

else()

endif()

set_target_properties( NFPluginLoader PROPERTIES OUTPUT_NAME_DEBUG "NFPluginLoader")
set_target_properties( NFPluginLoader PROPERTIES PREFIX "")
set_target_properties( NFPluginLoader PROPERTIES
	FOLDER "NFComm/NFPluginLoader"
	ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
	RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
	LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )


add_definitions(-D_X64 -D_CONSOLE)