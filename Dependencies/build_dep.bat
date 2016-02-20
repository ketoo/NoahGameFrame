@echo off

echo Building dependencies...

REM extact mysql libs
echo Extract mysql libs
"..\\Tool\\7z.exe" x mysql_libs.zip -Y

echo "Extract mysql++ libs"
"..\\Tool\\7z.exe" x mysqlpp_libs.zip -Y

echo Copy mysql libs to Debug directory
copy lib\\Release\\libmysql.dll lib\\Debug\\ /Y
copy lib\\Release\\libmysql.lib lib\\Debug\\ /Y
copy lib\\Release\\libmysqlclient.a lib\\Debug\\ /Y
copy lib\\Release\\libmysqlclient.so lib\\Debug\\ /Y
copy lib\\Release\\mysqlclient.lib lib\\Debug\\ /Y

REM extract libevent libs
echo Compiling libevent
"..\\Tool\\7z.exe" x libevent-2.0.22-stable.tar.gz -Y
"..\\Tool\\7z.exe" x libevent-2.0.22-stable.tar -Y
del libevent-2.0.22-stable.tar

if exist libevent rd libevent
rename libevent-2.0.22-stable libevent
rd libevent-2.0.22-stable

"..\\Tool\\7z.exe" x libevent_libs.zip -Y
copy lib\\Release\\libevent.a lib\\Debug\\ /Y
copy lib\\Release\\libevent.lib lib\\Debug\\ /Y
copy lib\\Release\\libevent_core.a lib\\Debug\\ /Y
copy lib\\Release\\libevent_core.lib lib\\Debug\\ /Y

REM extract curl libs
if exist curl rd curl
"..\\Tool\\7z.exe" x curl-7.37.1.zip -Y
rename curl-7.37.1 curl
rd curl-7.37.1

"..\\Tool\\7z.exe" x curl_libs.zip -Y
