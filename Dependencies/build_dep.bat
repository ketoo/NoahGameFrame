@echo off

echo Building dependencies...

echo Compiling libevent
"..\\Tool\\7z.exe" x libevent-2.0.22-stable.tar.gz -Y
"..\\Tool\\7z.exe" x libevent-2.0.22-stable.tar -Y
del libevent-2.0.22-stable.tar

if exist libevent rd /S /Q libevent
rename libevent-2.0.22-stable libevent
rd /S /Q libevent-2.0.22-stable

echo extract libevent libs
"..\\Tool\\7z.exe" x libevent_libs.zip -Y
copy lib\\Release\\libevent.a lib\\Debug\\ /Y
copy lib\\Release\\libevent.lib lib\\Debug\\ /Y
copy lib\\Release\\libevent_core.a lib\\Debug\\ /Y
copy lib\\Release\\libevent_core.lib lib\\Debug\\ /Y
