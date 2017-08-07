@echo off
echo Building dependencies...

if not exist lib\Debug mkdir lib\Debug
if not exist lib\Release mkdir lib\Release

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
cd libevent
nmake /f Makefile.nmake
copy libevent.lib ..\\lib\\Debug\\ /Y
copy libevent.lib ..\\lib\\Release\\ /Y
copy libevent_core.lib ..\\lib\\Debug\\ /Y
copy libevent_core.lib ..\\lib\\Release\\ /Y
cd ..


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
