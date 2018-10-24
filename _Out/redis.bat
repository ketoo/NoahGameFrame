cd /d %~dp0
cd Redis-x64-3.2.100

echo Starting redis-server...

redis-server.exe "redis.windows.conf"