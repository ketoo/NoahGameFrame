rmdir vcpkg

git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg


.\bootstrap-vcpkg.bat

vcpkg install libevent:x64-windows
vcpkg install protobuf:x64-windows
vcpkg install lua:x64-windows

cd..