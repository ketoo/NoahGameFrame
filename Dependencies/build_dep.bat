@echo off

echo Building dependencies...

REM extract libevent libs
if exist libevent rd /S /Q libevent
echo Compiling libevent
"..\\Tool\\7z.exe" x libevent.zip -Y

echo Compiling mysql
"..\\Tool\\7z.exe" x mysqlpp_libs.zip -Y
"..\\Tool\\7z.exe" x mysql_libs.zip -Y

copy lib\\Debug\\mysqlpp_d.dll ..\\_Out\\Debug\\ /Y
copy lib\\Release\\mysqlpp.dll ..\\_Out\\Release\\ /Y

copy lib\\Release\\libmysql.dll ..\\_Out\\Debug\\ /Y
copy lib\\Release\\libmysql.dll ..\\_Out\\Release\\ /Y

copy lib\\Release\\mysqlclient.lib lib\\Debug\\ /Y
copy lib\\Release\\mysqlpp.lib lib\\Debug\\ /Y
copy lib\\Release\\libmysql.lib lib\\Debug\\ /Y


"..\\Tool\\7z.exe" x gperftools-2.5.zip -Y