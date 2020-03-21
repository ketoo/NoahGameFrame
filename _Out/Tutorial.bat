cd /d %~dp0
cd Debug

echo Starting NFTutorialServer...
start "NFTutorialServer" "NFServer.exe" "Server=NFGameServer" "ID=6" "Plugin=Tutorial.xml"