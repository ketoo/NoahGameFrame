

sysOS=`uname -s`



rm -rf vcpkg

git clone https://github.com/Microsoft/vcpkg.git

cd vcpkg

./bootstrap-vcpkg.sh

if [ $sysOS == "Darwin" ];then

    ./vcpkg install libevent:x64-osx
    ./vcpkg install protobuf:x64-osx
    ./vcpkg install lua:x64-osx
    ./vcpkg install sdl2:x64-osx

elif [ $sysOS == "Linux" ];then
    ./vcpkg install libevent:x64-linux
    ./vcpkg install protobuf:x64-linux
    ./vcpkg install lua:x64-linux
    ./vcpkg install sdl2:x64-linux


fi


cd ..