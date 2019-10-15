rmdir vcpkg
rmdir libevent
rmdir lua
rmdir protobuf
rmdir concurrentqueue
rmdir ajson

git clone https://github.com/cameron314/concurrentqueue.git
git clone https://github.com/lordoffox/ajson.git

git clone https://github.com/Microsoft/vcpkg.git

cd vcpkg

#vcpkg remove easyloggingpp:x64-windows-static

.\bootstrap-vcpkg.bat

vcpkg list

vcpkg install libevent:x64-windows
vcpkg install protobuf:x64-windows
vcpkg install lua:x64-windows
vcpkg install gtest:x64-windows

xcopy installed\x64-windows\tools\protobuf\protoc.exe ..\..\NFComm\NFMessageDefine\ /s /e /Y

vcpkg integrate install

cd..