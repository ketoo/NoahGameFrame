cd /d %~dp0

@echo off

copy ..\\..\\..\\Dependencies\\lib\\Release\\libmysql.dll NFGameServer1\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Release\\libmysql.dll NFLoginServer\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Release\\libmysql.dll NFMasterServer\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Release\\libmysql.dll NFProxyServer1\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Release\\libmysql.dll NFWorldServer\\ /Y

copy ..\\..\\..\\Dependencies\\lib\\Release\\mysqlpp.dll NFGameServer1\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Release\\mysqlpp.dll NFLoginServer\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Release\\mysqlpp.dll NFMasterServer\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Release\\mysqlpp.dll NFProxyServer1\\ /Y
copy ..\\..\\..\\Dependencies\\lib\\Release\\mysqlpp.dll NFWorldServer\\ /Y

cd  NFMasterServer

echo Starting NFMasterServer...
start "" "NFPluginLoader.exe"

choice /t 2 /d y /n >nul
cd  ..
cd  NFWorldServer
echo Starting NFWorldServer...
start "" "NFPluginLoader.exe"


choice /t 5 /d y /n >nul

cd  ..
cd  NFLoginServer
echo Starting NFLoginServer...
start "" "NFPluginLoader.exe"

choice /t 2 /d y /n >nul

cd  ..
cd  NFGameServer1
echo Starting NFGameServer1...
start "" "NFPluginLoader.exe"

choice /t 4 /d y /n >nul

cd  ..
cd  NFProxyServer1
echo Starting NFProxyServer1...
start "" "NFPluginLoader.exe"






