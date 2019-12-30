cd /d %~dp0
cd Release

echo Starting NFMasterServer...
start "NFMasterServer" "NFServer.exe" "Server=MasterServer" "ID=3" "Plugin=Plugin.xml"

choice /t 2 /d y /n >nul
echo Starting NFWorldServer...
start "NFWorldServer" "NFServer.exe" "Server=WorldServer" "ID=7" "Plugin=Plugin.xml"


choice /t 5 /d y /n >nul

echo Starting NFLoginServer...
start "NFLoginServer" "NFServer.exe" "Server=LoginServer" "ID=4" "Plugin=Plugin.xml"

choice /t 5 /d y /n >nul

echo Starting NFDBServer...
start "NFDBServer" "NFServer.exe" "Server=DBServer" "ID=8" "Plugin=Plugin.xml"


choice /t 2 /d y /n >nul

echo Starting NFGameServer...
start "NFGameServer" "NFServer.exe" "Server=GameServer" "ID=6" "Plugin=Plugin.xml"

choice /t 2 /d y /n >nul


echo Starting NFProxyServer...
start "NFProxyServer" "NFServer.exe" "Server=ProxyServer" "ID=5" "Plugin=Plugin.xml"






