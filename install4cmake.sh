# make sure cmake is installed
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

#g++  sudo apt-get install g++
#unzip sudo apt-get install unzip

#compile dep libraries
rm -rf ./_Out/Debug
rm -rf ./_Out/Release

cd ./Dependencies
chmod -R 755 build_dep.sh
./build_dep.sh
cd ../

./buildServer.sh

#pwd
