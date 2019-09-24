echo Building dependencies...

rm -rf protobuf
rm -rf libevent
rm -rf lua
rm -rf ajson
rm -rf concurrentqueue

rm -rf lib

sysOS=`uname -s`

cmake --version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install cmake first."
    echo "[ubuntu] apt-get install cmake or [centos] yum install cmake or [mac] brew install cmake"
    exit 1
fi

unzip -v
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install unzip first."
    echo "[ubuntu] sudo apt-get install unzip or [centos] yum install unzip or [mac] brew install unzip"
    exit 1
fi

g++ --version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install g++ first."
    echo "[ubuntu] sudo apt-get install g++ or [centos] yum install g++ or [mac] brew install g++"
    exit 1
fi

automake --version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install automake first."
    echo "[ubuntu] sudo apt-get install automake or [centos] yum install automake or [mac] brew install automake"
    exit 1
fi

wget --version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install wget first."
    echo "[ubuntu] sudo apt-get install wget [centos] yum install wget or [mac] brew install wget"
    exit 1
fi

openssl version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install openssl first."
    echo "[ubuntu] sudo apt-get install libssl-dev or [centos] yum install libssl-dev or [mac] brew install openssl"
    exit 1
fi


git submodule update --init --recursive

rm -rf ./lib
mkdir -p lib/Debug/
mkdir -p lib/Release/

if [ $sysOS == "Darwin" ];then
    echo "what are you want to do???"
elif [ $sysOS == "Linux" ];then
	sudo apt-get install libreadline6-dev 
	sudo apt-get install libncurses5-dev
fi


#cd openssl-1.1.0h
#rm -rf *.a
#chmod -R 755 *
#./config no-shared
#make depend
#make
#cp -r -f ./*.a ../lib/Debug/
#cp -r -f ./*.a ../lib/Release/
#cd ../


# compiling libevent
echo Start to build  libevent...................................................
cd libevent
chmod 777 *.sh
./autogen.sh

chmod +x ./configure
#./configure --disable-shared --disable-openssl
./configure --disable-shared --disable-openssl
make

cp -R -f ./.libs/*.a ../lib/Debug/
cp -R -f ./.libs/*.a ../lib/Release/

echo finished libevent..................................................
cd ../


# compiling protobuf
echo Start to build protobuf...................................................
cd protobuf
chmod 777 *.sh
./autogen.sh
./configure --disable-shared --with-protoc
make
# make check

cp -r -f ./src/.libs/*.a ../lib/Debug/
cp -r -f ./src/.libs/*.a ../lib/Release/

cp -r -f ./src/protoc ../../NFComm/NFMessageDefine/protoc

cd ../

echo finished protobuf..................................................

# compiling lua
echo Building lua...................................................

rm -rf lua
wget http://www.lua.org/ftp/lua-5.3.5.tar.gz
mkdir lua
tar zxf lua-5.3.5.tar.gz
mv lua-5.3.5 lua
cd lua

if [ $sysOS == "Darwin" ];then
    make macosx test
elif [ $sysOS == "Linux" ];then
    make linux test
fi

cp -r -f ./src/*.a ../lib/Debug/
cp -r -f ./src/*.a ../lib/Release/

cd ../

echo Building lua finish...

# compiling hiredis
echo Building hiredis...
chmod 777 build_hiredis.sh
./build_hiredis.sh
echo Building hiredis finish...




# TODO: other libs
#unzip -o gperftools-2.7.zip -d ./
#cd gperftools-2.7
#chmod -R 755 *
#./configure --enable-frame-pointers --disable-heap-profiler --disable-heap-checker
#make

#cp -R -f ./.libs/*.a ../lib/Debug/
#cp -R -f ./.libs/*.a ../lib/Release/


#-ltcmalloc
#-lprofiler

# back to main dir
pwd
