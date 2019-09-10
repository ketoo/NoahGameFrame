cd /d %~dp0
cd Release
echo Starting NFTutorial...
start "NFTutorialServer" "NFPluginLoader.exe" "Server=TutorialServer" "ID=5" "Tutorial5.xml"