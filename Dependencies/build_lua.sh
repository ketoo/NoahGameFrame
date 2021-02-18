

rm -rf lua-5.3.6

wget http://www.lua.org/ftp/lua-5.3.6.tar.gz
tar zxf lua-5.3.6.tar.gz
cd lua-5.3.6


sysOS=`uname -s`

if [ $sysOS == "Linux" ];then
make linux
cp ./src/*.h ../vcpkg/installed/x64-linux/include/
cp ./src/*.hpp ../vcpkg/installed/x64-linux/include/

elif [ $sysOS == "Darwin" ];then
make macosx
cp ./src/*.h ../vcpkg/installed/x64-osx/include/
cp ./src/*.hpp ../vcpkg/installed/x64-osx/include/
fi


cp ./src/*.a ../lib/Debug/
cp ./src/*.a ../lib/Release/

cd ../
