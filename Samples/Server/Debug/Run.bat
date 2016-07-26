cd /d %~dp0

@echo off
copy ..\\..\\..\\_Out\\Comm\\Debug\\NFPluginLoader_d.exe NFLoginServer\\ /Y
copy ..\\..\\..\\_Out\\Comm\\Debug\\NFPluginLoader_d.exe NFMasterServer\\ /Y
copy ..\\..\\..\\_Out\\Comm\\Debug\\NFPluginLoader_d.exe NFProxyServer1\\ /Y
copy ..\\..\\..\\_Out\\Comm\\Debug\\NFPluginLoader_d.exe NFWorldServer\\ /Y
copy ..\\..\\..\\_Out\\Comm\\Debug\\NFPluginLoader_d.exe NFGameServer1\\ /Y

copy ..\\..\\..\\Dependencies\\lib\\Debug\\libmysql.dll NFGameServer1\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Debug\\libmysql.dll NFLoginServer\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Debug\\libmysql.dll NFMasterServer\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Debug\\libmysql.dll NFProxyServer1\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Debug\\libmysql.dll NFWorldServer\\ /Y

copy ..\\..\\..\\Dependencies\\lib\\Debug\\mysqlpp_d.dll NFGameServer1\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Debug\\mysqlpp_d.dll NFLoginServer\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Debug\\mysqlpp_d.dll NFMasterServer\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Debug\\mysqlpp_d.dll NFProxyServer1\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Debug\\mysqlpp_d.dll NFWorldServer\\ /Y

cd  NFMasterServer

echo Starting NFMasterServer...
start "" "NFPluginLoader_d.exe"

choice /t 2 /d y /n >nul
cd  ..
cd  NFWorldServer
echo Starting NFWorldServer...
start "" "NFPluginLoader_d.exe"


choice /t 5 /d y /n >nul

cd  ..
cd  NFLoginServer
echo Starting NFLoginServer...
start "" "NFPluginLoader_d.exe"

choice /t 2 /d y /n >nul

cd  ..
cd  NFGameServer1
echo Starting NFGameServer1...
start "" "NFPluginLoader_d.exe"

choice /t 4 /d y /n >nul

cd  ..
cd  NFProxyServer1
echo Starting NFProxyServer1...
start "" "NFPluginLoader_d.exe"






