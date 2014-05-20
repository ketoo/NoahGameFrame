@echo Éú³ÉNFSDK


xcopy /Y 		.\Config\*.h						..\..\..\Dev\Dependency\NFComm\Config
xcopy /Y 		.\NFConfigPlugin\*.h				..\..\..\Dev\Dependency\NFComm\NFConfigPlugin
xcopy /Y 		.\NFCore\*.h						..\..\..\Dev\Dependency\NFComm\NFCore
xcopy /Y 		.\NFEventProcessPlugin\*.h			..\..\..\Dev\Dependency\NFComm\NFEventProcessPlugin
xcopy /Y 		.\NFKernelPlugin\*.h				..\..\..\Dev\Dependency\NFComm\NFKernelPlugin
xcopy /Y 		.\NFKernelPlugin\NFContainerModule\*.h				..\..\..\Dev\Dependency\NFComm\NFKernelPlugin\NFContainerModule
xcopy /Y 		.\NFKernelPlugin\NFGridModule\*.h				..\..\..\Dev\Dependency\NFComm\NFKernelPlugin\NFGridModule
xcopy /Y 		.\NFKernelPlugin\NFKernelModule\*.h				..\..\..\Dev\Dependency\NFComm\NFKernelPlugin\NFKernelModule


xcopy /Y 		..\_Out\Comm\Debug\NFKernelPlugin_Static_d.lib				..\..\..\Dev\Dependency\NFComm\Lib
xcopy /Y 		..\_Out\Comm\Release\NFKernelPlugin_Static.lib				..\..\..\Dev\Dependency\NFComm\Lib

xcopy /Y 		..\_Out\Comm\Debug\NFConfigPlugin_Static_d.lib				..\..\..\Dev\Dependency\NFComm\Lib
xcopy /Y 		..\_Out\Comm\Release\NFConfigPlugin_Static.lib				..\..\..\Dev\Dependency\NFComm\Lib

xcopy /Y 		..\_Out\Comm\Debug\NFCore_Static_d.lib				..\..\..\Dev\Dependency\NFComm\Lib
xcopy /Y 		..\_Out\Comm\Release\NFCore_Static.lib				..\..\..\Dev\Dependency\NFComm\Lib

xcopy /Y 		..\_Out\Comm\Debug\NFEventProcessPlugin_Static_d.lib				..\..\..\Dev\Dependency\NFComm\Lib
xcopy /Y 		..\_Out\Comm\Release\NFEventProcessPlugin_Static.lib				..\..\..\Dev\Dependency\NFComm\Lib

xcopy /Y 		..\_Out\Comm\Debug\Theron_Static_d.lib			..\..\..\Dev\Dependency\NFComm\Lib
xcopy /Y 		..\_Out\Comm\Release\Theron_Static.lib				..\..\..\Dev\Dependency\NFComm\Lib



xcopy /Y 		.\NFPluginModule\NFIKernelModule.h				..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFIKernelModule.h
xcopy /Y 		.\NFPluginModule\NFIConcurrentModule.h				..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFIConcurrentModule.h
xcopy /Y 		.\NFPluginModule\NFILogModule.h				..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFILogModule.h
xcopy /Y 		.\NFPluginModule\NFIUUIDModule.h				..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFIUUIDModule.h
xcopy /Y 		.\NFPluginModule\NFIEventProcessModule.h				..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFIEventProcessModule.h
xcopy /Y 		.\NFPluginModule\NFIElementInfoModule.h				..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFIElementInfoModule.h
xcopy /Y 		.\NFPluginModule\NFILogicClassModule.h				..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFILogicClassModule.h
xcopy /Y 		.\NFPluginModule\NFIPlugin.h			..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFIPlugin.h
xcopy /Y 		.\NFPluginModule\NFIPluginManager.h			..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFIPluginManager.h
xcopy /Y 		.\NFPluginModule\NFIActor.h			..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFIActor.h
xcopy /Y 		.\NFPluginModule\NFIActorManager.h			..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFIActorManager.h
xcopy /Y 		.\NFPluginModule\NFILogicModule.h			..\..\..\Dev\Dependency\NFComm\NFPluginModule\NFILogicModule.h





@echo ...
