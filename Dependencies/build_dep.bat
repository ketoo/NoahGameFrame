RMDIR /Q/S vcpkg

RMDIR /Q/S libevent

RMDIR /Q/S lua

RMDIR /Q/S protobuf

RMDIR /Q/S concurrentqueue

RMDIR /Q/S ajson

git clone https://github.com/cameron314/concurrentqueue.git
git clone https://github.com/lordoffox/ajson.git

git clone https://github.com/Microsoft/vcpkg.git

cd vcpkg

call .\bootstrap-vcpkg.bat

vcpkg install libevent:x64-windows-static

vcpkg install protobuf:x64-windows-static

vcpkg install lua:x64-windows-static

vcpkg install gtest:x64-windows-static


xcopy installed\x64-windows\tools\protobuf\protoc.exe ..\..\NFComm\NFMessageDefine\ /s /e /Y

cd..