
git submodule update --init --recursive

rd /s /q vcpkg

git clone https://github.com/Microsoft/vcpkg.git

cd vcpkg

call .\bootstrap-vcpkg.bat

vcpkg install libevent:x64-windows-static

vcpkg install protobuf:x64-windows-static
vcpkg install lua:x64-windows-static --overlay-ports=..\vcpkg_port\lua_3.5
vcpkg install sdl2:x64-windows-static

cd..

xcopy vcpkg\installed\x64-windows-static\lib lib\Release\ /s /e /Y
xcopy vcpkg\installed\x64-windows-static\bin ..\_Out\Release\  /s /e /Y
ren lib\Release\SDL2-static.lib SDL2.lib

xcopy vcpkg\installed\x64-windows-static\debug\lib lib\Debug\  /s /e /Y
xcopy vcpkg\installed\x64-windows-static\debug\bin ..\_Out\Debug\  /s /e /Y
ren lib\Debug\SDL2-staticd.lib SDL2d.lib
ren lib\Debug\eventd.lib event.lib
ren lib\Debug\event_cored.lib event_core.lib
ren lib\Debug\event_extrad.lib event_extra.lib


xcopy vcpkg\installed\x64-windows-static\tools\protobuf\* ..\NFComm\NFMessageDefine\ /s /e /Y
cd ..\NFComm\NFMessageDefine
.\cpp.bat



cd..