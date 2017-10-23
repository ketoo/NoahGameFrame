cd Debug

echo Starting NFMasterServer...
start "NFMasterServer" "NFPluginLoader_d.exe" "Server=MasterServer" "ID=3"

choice /t 2 /d y /n >nul

echo Starting NFLoginServer...
start "NFLoginServer" "NFPluginLoader_d.exe" "Server=LoginServer" "ID=4"





