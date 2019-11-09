cd /d %~dp0
cd Release

echo Starting NFMasterServer...
start "NFMasterServer" "NFPluginLoader.exe" "Server=MasterServer" "ID=3" "Plugin.xml"

choice /t 2 /d y /n >nul
echo Starting NFWorldServer...
start "NFWorldServer" "NFPluginLoader.exe" "Server=WorldServer" "ID=7" "Plugin.xml"


choice /t 5 /d y /n >nul

echo Starting NFLoginServer...
start "NFLoginServer" "NFPluginLoader.exe" "Server=LoginServer" "ID=4" "Plugin.xml"

choice /t 5 /d y /n >nul

echo Starting NFDBServer...
start "NFDBServer" "NFPluginLoader.exe" "Server=DBServer" "ID=8" "Plugin.xml"


choice /t 2 /d y /n >nul

echo Starting NFGameServer...
start "NFGameServer" "NFPluginLoader.exe" "Server=GameServer" "ID=6" "Plugin.xml"

choice /t 2 /d y /n >nul


echo Starting NFProxyServer...
start "NFProxyServer" "NFPluginLoader.exe" "Server=ProxyServer" "ID=5" "Plugin.xml"