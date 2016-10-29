cd Debug

echo Starting NFMasterServer...
start "NFMasterServer" "NFPluginLoader_r.exe" "Server=MasterServer" "ID=3"

choice /t 2 /d y /n >nul
echo Starting NFWorldServer...
start "NFWorldServer" "NFPluginLoader_r.exe" "Server=NFWorldServer" "ID=7"


choice /t 5 /d y /n >nul

echo Starting NFLoginServer...
start "NFLoginServer" "NFPluginLoader_r.exe" "Server=NFLoginServer" "ID=4"

choice /t 2 /d y /n >nul

echo Starting NFGameServer...
start "NFGameServer" "NFPluginLoader_r.exe" "Server=NFGameServer" "ID=6"

choice /t 4 /d y /n >nul

echo Starting NFProxyServer...
start "NFProxyServer" "NFPluginLoader_r.exe" "Server=NFProxyServer" "ID=5"






