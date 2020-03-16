
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
    brew install gcc@7
elif [ $sysOS == "Linux" ];then
    if [ $DISTRO == "Debian" ] || [ $DISTRO == "Ubuntu" ] || [ $DISTRO == "Raspbian" ]; then
        sudo apt-get install g++-7
        sudo apt-get install libtool
        sudo apt-get install libstdc++-static
        sudo apt-get install libreadline6-dev 
        sudo apt-get install libncurses5-dev
    else
        sudo yum -y install centos-release-scl
		sudo yum -y install devtoolset-7
		sudo scl enable devtoolset-7 bash 
        sudo yum -y install libtool
        sudo yum -y install readline-devel
        sudo yum -y install ncurses-devel
        sudo yum -y install libstdc++-static
    fi

fi

mkdir lib
mkdir ./lib/Release/
mkdir ./lib/Debug/

git submodule update --init --recursive

chmod 777 *.sh

./build_hiredis.sh
./build_vcpkg.sh

if [ $sysOS == "Darwin" ];then
    cp -r -f ./vcpkg/installed/x64-osx/lib/* ./lib/Release/
    cp -r -f ./vcpkg/installed/x64-osx/debug/lib/* ./lib/Debug/

    cp -r -f ./vcpkg/installed/x64-osx/tools/protobuf/* ../NFComm/NFMessageDefine/

elif [ $sysOS == "Linux" ];then
    cp -r -f ./vcpkg/installed/x64-linux/lib/* ./lib/Release/
    cp -r -f ./vcpkg/installed/x64-linux/debug/lib/* ./lib/Debug/

    cp -r -f ./vcpkg/installed/x64-linux/tools/protobuf/* ../NFComm/NFMessageDefine/
fi
