cd Debug

echo Starting NFMasterServer...
start "NFMasterServer" "DEBUG_MASTER.exe" "Server=MasterServer" "ID=3" "PluginX.xml"

choice /t 2 /d y /n >nul
echo Starting NFWorldServer...
start "NFWorldServer" "DEBUG_WORLD.exe" "Server=WorldServer" "ID=7" "PluginX.xml"


choice /t 5 /d y /n >nul
echo Starting NFLoginServer...
start "NFLoginServer" "DEBUG_LOGIN.exe" "Server=LoginServer" "ID=4" "PluginX.xml"

choice /t 2 /d y /n >nul
echo Starting NFGameServer...
start "NFGameServer" "DEBUG_GAME.exe" "Server=GameServer" "ID=6" "PluginX.xml"

choice /t 2 /d y /n >nul
echo Starting NFProxyServer...
start "NFProxyServer" "DEBUG_PROXY.exe" "Server=ProxyServer" "ID=5" "PluginX.xml"

choice /t 2 /d y /n >nul
echo Starting NFWorldServer...
start "NFWorldServer1" "DEBUG_WORLD1.exe" "Server=WorldServer" "ID=71" "PluginX.xml"
choice /t 2 /d y /n >nul
echo Starting NFGameServer...
start "NFGameServer1" "DEBUG_GAME1.exe" "Server=GameServer" "ID=61" "PluginX.xml"
choice /t 2 /d y /n >nul
echo Starting NFProxyServer...
start "NFProxyServer1" "DEBUG_PROXY1.exe" "Server=ProxyServer" "ID=51" "PluginX.xml"





