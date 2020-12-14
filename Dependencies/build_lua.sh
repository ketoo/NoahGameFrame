

curl -R -O http://www.lua.org/ftp/lua-5.3.6.tar.gz
tar zxf lua-5.3.6.tar.gz
cd lua-5.3.6


sysOS=`uname -s`

if [ $sysOS == "Darwin" ];then
make linux
cp ./src/*.h ../vcpkg/installed/x64-osx/include/

elif [ $sysOS == "Linux" ];then
make macosx
cp ./src/*.h ../vcpkg/installed/x64-osx/linux/
fi


cp ./src/*.a ../lib/Debug/
cp ./src/*.a ../lib/Release/

cd ../