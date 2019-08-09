cd /d %~dp0
cd Release
echo Starting NFTutorial...
start "NFTutorialServer" "NFPluginLoader.exe" "Server=TutorialServer" "ID=0" "Tutorial4.xml"