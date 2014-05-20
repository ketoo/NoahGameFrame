cd /d %~dp0

@echo off

cd  ..
cd  Release\Redis\redisbin
echo Starting Redis...
start "" "redis-server.exe"

choice /t 2 /d y /n >nul
cd  ..
cd  ..
cd  ..
cd  Debug\NFMasterServer

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

choice /t 2 /d y /n >nul

cd  ..
cd  NFGameServer2
echo Starting NFGameServer2...
start "" "NFPluginLoader_d.exe"

choice /t 4 /d y /n >nul

cd  ..
cd  NFProxyServer1
echo Starting NFProxyServer1...
start "" "NFPluginLoader_d.exe"






