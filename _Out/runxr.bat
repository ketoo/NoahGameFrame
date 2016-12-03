cd Release

echo Starting NFMasterServer...
start "NFMasterServer" "NFPluginLoader.exe" "Server=MasterServer" "ID=3" "PluginX.xml"

choice /t 2 /d y /n >nul
echo Starting NFWorldServer...
start "NFWorldServer" "NFPluginLoader.exe" "Server=WorldServer" "ID=7" "PluginX.xml"


choice /t 5 /d y /n >nul

echo Starting NFLoginServer...
start "NFLoginServer" "NFPluginLoader.exe" "Server=LoginServer" "ID=4" "PluginX.xml"

choice /t 2 /d y /n >nul

echo Starting NFGameServer...
start "NFGameServer" "NFPluginLoader.exe" "Server=GameServer" "ID=6" "PluginX.xml"

choice /t 4 /d y /n >nul

echo Starting NFProxyServer...
start "NFProxyServer" "NFPluginLoader.exe" "Server=ProxyServer" "ID=5" "PluginX.xml"