
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


sysOS=`uname -s`

if [ $sysOS == "Darwin" ];then
    brew install gcc6
elif [ $sysOS == "Linux" ];then
    if [ $DISTRO == "Debian" ] || [ $DISTRO == "Ubuntu" ] || [ $DISTRO == "Raspbian" ]; then
        sudo apt-get install gcc6
        sudo apt-get install libtool
        sudo apt-get install libstdc++-static
        sudo apt-get install libreadline6-dev 
        sudo apt-get install libncurses5-dev
    else
        sudo yum -y install gcc6
        sudo yum -y install libtool
        sudo yum -y install readline-devel
        sudo yum -y install ncurses-devel
        sudo yum -y install libstdc++-static
    fi

fi

git submodule update --init --recursive

rm -rf vcpkg

git clone https://github.com/Microsoft/vcpkg.git

cd vcpkg

./bootstrap-vcpkg.sh

if [ $sysOS == "Darwin" ];then

    ./vcpkg install libevent:x64-osx
    ./vcpkg install protobuf:x64-osx
    ./vcpkg install lua:x64-osx
    ./vcpkg install sdl2:x64-osx
    ./vcpkg install gtest:x64-osx
    ./vcpkg install opengl:x64-osx

elif [ $sysOS == "Linux" ];then
    ./vcpkg install libevent:x64-linux
    ./vcpkg install protobuf:x64-linux
    ./vcpkg install lua:x64-linux
    ./vcpkg install sdl2:x64-linux
    ./vcpkg install gtest:x64-linux


fi


cd ..

mkdir ./lib/Release
mkdir ./lib/Debug

if [ $sysOS == "Darwin" ];then
    cp -r -f ./vcpkg/installed/x64-osx/lib/* ./lib/Release/
    cp -r -f ./vcpkg/installed/x64-osx/debug/lib/* ./lib/Debug/

    cp -r -f ./vcpkg/installed/x64-osx/tools/protobuf/* ../NFComm/NFMessageDefine/

elif [ $sysOS == "Linux" ];then
    cp -r -f ./vcpkg/installed/x64-linux/lib/* ./lib/Release/
    cp -r -f ./vcpkg/installed/x64-linux/debug/lib/* ./lib/Debug/

    cp -r -f ./vcpkg/installed/x64-linux/tools/protobuf/* ../NFComm/NFMessageDefine/
fi

cd ..

./build_hiredis.sh
