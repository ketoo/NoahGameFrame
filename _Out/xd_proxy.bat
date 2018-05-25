cd Debug

choice /t 2 /d y /n >nul

echo Starting NFProxyServer...
start "NFProxyServer" "NFPluginLoader_d.exe" "Server=ProxyServer" "ID=5" "PluginX.xml"






