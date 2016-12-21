cd Debug

echo Starting NFMasterServer...
start "NFMasterServer" "NFPluginLoader_d.exe" "Server=MasterServer" "ID=3" "PluginX.xml"

choice /t 2 /d y /n >nul
echo Starting NFWorldServer...
start "NFWorldServer" "NFPluginLoader_d.exe" "Server=WorldServer" "ID=7" "PluginX.xml"


choice /t 5 /d y /n >nul

echo Starting NFLoginServer...
start "NFLoginServer" "NFPluginLoader_d.exe" "Server=LoginServer" "ID=4" "PluginX.xml"

choice /t 2 /d y /n >nul

echo Starting NFGameServer...
start "NFGameServer" "NFPluginLoader_d.exe" "Server=GameServer" "ID=6" "PluginX.xml"

choice /t 4 /d y /n >nul

echo Starting NFProxyServer...
start "NFProxyServer" "NFPluginLoader_d.exe" "Server=ProxyServer" "ID=5" "PluginX.xml"






