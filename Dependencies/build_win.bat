@echo off

rem first extract the mysql-connector-c to "C:\Program Files\MySQL\MySQL Connector C 6.1"
rem you can find all the zip file from tar folder

echo "***************************************************************************"
echo 
echo "********************Current only debug mode for windows********************"
echo 
echo "***************************************************************************"

rem ************************check build environment******************************************
if not "%Platform%" == "X64" (
	echo Please use the Visual Studio x64 command prompt Shell
	goto exit
)


rem ************************copy the mysql C lib******************************************
echo Check and copy mysql-connector-c from C:\Program Files\MySQL\MySQL Connector C 6.1

set MySqlCLib="C:\Program Files\MySQL\MySQL Connector C 6.1"

if not exist %MySqlCLib% (
	echo Please install MySQL Connector C 6.1 first!!! 
	goto exit
)

copy %MySqlCLib%\lib\libmysql.lib .\lib\Debug\ /Y
copy %MySqlCLib%\lib\libmysql.dll .\lib\Debug\ /Y

if "%VisualStudioVersion%" == "12.0" (
	copy %MySqlCLib%\lib\vs12\mysqlclient.lib .\lib\Debug\ /Y
) else if "%VisualStudioVersion%" == "11.0" (
	copy %MySqlCLib%\lib\vs11\mysqlclient.lib .\lib\Debug\ /Y
)

rem ************************build mysql++******************************************
echo Building the mysql++ ...
cd mysqlpp\vc2008

if not exist mysql++_mysqlpp.vcxproj (
	devenv mysql++_mysqlpp.vcproj /upgrade
)
devenv mysql++_mysqlpp.vcxproj /Build "Debug|x64"

copy .\x64\Debug\mysqlpp_d.lib ..\..\lib\Debug\ /Y
copy .\x64\Debug\mysqlpp_d.dll ..\..\lib\Debug\ /Y

cd ..\..

rem *******************build libevent**********************************
echo Building the libevent ...

cd %~dp0\libevent
nmake /f Makefile.nmake

copy .\libevent.lib ..\lib\Debug\ /Y
copy .\libevent_core.lib ..\lib\Debug\ /Y
copy .\libevent_extras.lib ..\lib\Debug\ /Y
cd ..

rem *******************build gperftools**********************************
echo Building the gperftools ...
cd gperftools\vsprojects\libtcmalloc_minimal
if not exist libtcmalloc_minimal.vcxproj (
	devenv libtcmalloc_minimal.vcproj /upgrade
)

devenv libtcmalloc_minimal.vcxproj /Build "Debug|Win32"

copy ..\..\Debug\libtcmalloc_minimal.lib	..\..\..\lib\Debug\ /Y
cd ..\..\..



rem *******************build curl**********************************
echo Building the curl ...
cd curl
if "%VisualStudioVersion%" == "14.0" (
	devenv projects\Windows\VC14\lib\libcurl.vcxproj /build "LIB Debug|x64"
	copy "build\Win64\VC14\LIB Debug\libcurld.lib"	..\lib\Debug\ /Y
) else if "%VisualStudioVersion%" == "12.0" (
	devenv projects\Windows\VC12\lib\libcurl.vcxproj /build "LIB Debug|x64"
	copy "build\Win64\VC12\LIB Debug\libcurld.lib"	..\lib\Debug\ /Y
)else if "%VisualStudioVersion%" == "11.0" (
	devenv projects\Windows\VC11\lib\libcurl.vcxproj /build "LIB Debug|x64"
	copy "build\Win64\VC11\LIB Debug\libcurld.lib"	..\lib\Debug\ /Y
)
cd ..


echo "******************done for all except the boost *****************************"

rem *******************build boost**********************************

CHOICE /C nY /N /M "Compile the boost? n"

if %ERRORLEVEL%==1 goto exit

echo Build eval boost ...

cd boost
call bootstrap.bat
.\b2 --toolset=msvc-12.0 link=static

md stage\lib\x64\
copy stage\lib\*.* stage\lib\x64\*.* /Y
cd ..


:exit
