# make sure cmake is installed
cmake --version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install cmake first."
    echo "[ubuntu] apt-get -y install cmake or [centos] yum -y install cmake or [mac] brew install cmake"
    exit 1
fi

unzip -v
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install unzip first."
    echo "[ubuntu] sudo apt-get -y install unzip or [centos] yum -y install unzip or [mac] brew install unzip"
    exit 1
fi

automake --version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install automake first."
    echo "[ubuntu] sudo apt-get -y install automake or [centos] yum -y install automake or [mac] brew install automake"
    exit 1
fi

if grep -Eqi "CentOS" /etc/issue || grep -Eq "CentOS" /etc/*-release; then
    DISTRO='CentOS'
elif grep -Eqi "Ubuntu" /etc/issue || grep -Eq "Ubuntu" /etc/*-release; then
    DISTRO='Ubuntu'
else
    DISTRO='unknow'
fi

sysOS=`uname -s`

if [ $sysOS == "Darwin" ];then
	gcc@7 --version
	if [ $? -ne 0 ]; then
		echo "[ERROR] Please install gcc@7 first."
        echo "[mac] brew install gcc@7"
        #exit 1
	fi
elif [ $sysOS == "Linux" ];then
    if [ $DISTRO == "Ubuntu" ]; then
		g++ --version
		if [ $? -ne 0 ]; then
			echo "[ERROR] Please install g++ first."
			echo "[Ubuntu] sudo apt-get -y install g++"
			exit 1
		fi
		g++-7 --version
		if [ $? -ne 0 ]; then
			echo "[ERROR] Please install g++-7 first."
			echo "[Ubuntu] sudo apt-get -y install g++-7"
			exit 1
		fi
	elif [ $DISTRO == "CentOS" ]; then
	    g++ --version
		if [ $? -ne 0 ]; then
			echo "[ERROR] Install g++ first, minimum version is 7.0.0"
            echo "[CentOS Step 1] yum install centos-release-scl -y"
            echo "[CentOS Step 2] yum install devtoolset-7 -y"
            echo "[CentOS Step 3] scl enable devtoolset-7 bash"
			exit 1
		fi
		currentver="$(gcc -dumpfullversion -dumpversion)"
        requiredver="7.0.0"
        if [ ! "$(printf '%s\n' "$requiredver" "$currentver" | sort -V | head -n1)" = "$requiredver" ]; then
            echo "[ERROR] Upgrate g++ first, minimum version is 7.0.0"
            echo "[CentOS Step 1] yum install centos-release-scl -y"
            echo "[CentOS Step 2] yum install devtoolset-7 -y"
            echo "[CentOS Step 3] scl enable devtoolset-7 bash"
			exit 1
        fi
	fi
fi
#g++  sudo apt-get -y install g++
#unzip sudo apt-get -y install unzip

#compile dep libraries
rm -rf ./_Out/Debug
rm -rf ./_Out/Release

cd ./Dependencies
chmod -R 755 build_dep.sh
./build_dep.sh
cd ../

./buildServer.sh

#pwd
