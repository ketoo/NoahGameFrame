start NFPluginLoader_d.exe ID=1 Server=MasterServer
choice /t 5 /d y /n >nul
start NFPluginLoader_d.exe ID=2 Server=WorldServer
choice /t 5 /d y /n >nul
start NFPluginLoader_d.exe ID=3 Server=ProxyServer
choice /t 5 /d y /n >nul
start NFPluginLoader_d.exe ID=4 Server=LoginServer
choice /t 5 /d y /n >nul
start NFPluginLoader_d.exe ID=5 Server=GameServer