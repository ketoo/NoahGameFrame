RMDIR /Q/S vcpkg

RMDIR /Q/S libevent

RMDIR /Q/S lua

RMDIR /Q/S protobuf

RMDIR /Q/S concurrentqueue

RMDIR /Q/S ajson

RMDIR /Q/S vcpkg/installedx64-windows/release

git clone https://github.com/cameron314/concurrentqueue.git
git clone https://github.com/lordoffox/ajson.git

git clone https://github.com/Microsoft/vcpkg.git

cd vcpkg

call .\bootstrap-vcpkg.bat

vcpkg install libevent:x64-windows

vcpkg install protobuf:x64-windows

vcpkg install lua:x64-windows

vcpkg install gtest:x64-windows

xcopy vcpkg\installed\x64-windows\lib lib\Release\ /s /e /Y
xcopy vcpkg\installed\x64-windows\bin ..\_Out\Release\  /s /e /Y

xcopy vcpkg\installed\x64-windows\debug\lib lib\Debug\  /s /e /Y
xcopy vcpkg\installed\x64-windows\debug\bin ..\_Out\Debug\  /s /e /Y


xcopy vcpkg\installed\x64-windows\tools\protobuf ..\NFComm\NFMessageDefine\ /s /e /Y

xcopy vcpkg\installed\x64-windows\debug\bin ..\_Out\Debug\



cd..