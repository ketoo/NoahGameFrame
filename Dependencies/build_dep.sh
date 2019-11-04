echo Building dependencies...

#rm -rf protobuf
#rm -rf libevent
rm -rf lua
#rm -rf ajson
#rm -rf concurrentqueue

	if grep -Eqi "CentOS" /etc/issue || grep -Eq "CentOS" /etc/*-release; then
        DISTRO='CentOS'
        PM='yum'
    elif grep -Eqi "Red Hat Enterprise Linux Server" /etc/issue || grep -Eq "Red Hat Enterprise Linux Server" /etc/*-release; then
        DISTRO='RHEL'
        PM='yum'
    elif grep -Eqi "Aliyun" /etc/issue || grep -Eq "Aliyun" /etc/*-release; then
        DISTRO='Aliyun'
        PM='yum'
    elif grep -Eqi "Fedora" /etc/issue || grep -Eq "Fedora" /etc/*-release; then
        DISTRO='Fedora'
        PM='yum'
    elif grep -Eqi "Debian" /etc/issue || grep -Eq "Debian" /etc/*-release; then
        DISTRO='Debian'
        PM='apt-get'
    elif grep -Eqi "Ubuntu" /etc/issue || grep -Eq "Ubuntu" /etc/*-release; then
        DISTRO='Ubuntu'
        PM='apt-get'
    elif grep -Eqi "Raspbian" /etc/issue || grep -Eq "Raspbian" /etc/*-release; then
        DISTRO='Raspbian'
        PM='apt-get'
    else
        DISTRO='unknow'
    fi



rm -rf lib

sysOS=`uname -s`

cmake --version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install cmake first."
    echo "[ubuntu, debian] apt-get install cmake or [centos] yum -y install cmake or [mac] brew install cmake"
    exit 1
fi

unzip -v
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install unzip first."
    echo "[ubuntu, debian] sudo apt-get install unzip or [centos] yum -y install unzip or [mac] brew install unzip"
    exit 1
fi

g++ --version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install g++ first."
    echo "[ubuntu, debian] sudo apt-get install g++ or [centos] yum -y install gcc-c++ or [mac] brew install g++"
    exit 1
fi

automake --version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install automake first."
    echo "[ubuntu, debian] sudo apt-get install automake or [centos] yum -y install automake or [mac] brew install automake"
    exit 1
fi

wget --version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install wget first."
    echo "[ubuntu, debian] sudo apt-get install wget [centos] yum -y install wget or [mac] brew install wget"
    exit 1
fi

openssl version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install openssl first."
    echo "[ubuntu, debian] sudo apt-get install libssl-dev or [centos] yum -y install libssl-dev or [mac] brew install openssl"
    exit 1
fi

cd ..
git submodule update --init --recursive
cd Dependencies

rm -rf ./lib
mkdir -p lib/Debug/
mkdir -p lib/Release/

if [ $sysOS == "Darwin" ];then
    echo "what are you want to do???"
elif [ $sysOS == "Linux" ];then
	if [ $DISTRO == "Debian" ] || [ $DISTRO == "Ubuntu" ] || [ $DISTRO == "Raspbian" ]; then
		sudo apt-get install libtool
		sudo apt-get install libstdc++-static
		sudo apt-get install libreadline6-dev 
		sudo apt-get install libncurses5-dev
	else
		sudo yum -y install libtool
		sudo yum -y install readline-devel
		sudo yum -y install ncurses-devel
        sudo yum -y install libstdc++-static
	fi
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
rm lua-5.3.5.tar.gz
wget http://www.lua.org/ftp/lua-5.3.5.tar.gz
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

echo Building lua finish.............................................

cd LuaIntf

cd ..

# compiling hiredis
echo Building hiredis.............................................
chmod 777 build_hiredis.sh
./build_hiredis.sh
echo Building hiredis finish.............................................




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
