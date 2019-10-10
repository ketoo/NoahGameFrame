rmdir vcpkg

git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg


.\bootstrap-vcpkg.bat

vcpkg list

vcpkg install libevent:x64-windows-static
vcpkg install protobuf:x64-windows-static
vcpkg install lua:x64-windows-static
vcpkg install gtest:x64-windows-static



vcpkg integrate install

cd..