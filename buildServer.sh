# make sure cmake is installed
# sudo apt-get update
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

cd BuildScript/linux/

#find . -name "*.o" | xargs rm -rf

chmod -R 755 ./BuildNF.CMake.Tools.sh
./BuildNF.CMake.Tools.sh
cd ../../

cd _Out/NFDataCfg/Tool/
chmod 755 ./NFFileProcess
chmod 755 ./copy_files.sh
./copy_files.sh
cd ..
cd ..
cd ..

cd BuildScript/linux/
chmod -R 755 ./BuildNF.CMake.Debug.sh
chmod -R 755 ./BuildNF.CMake.Release.sh
time ./BuildNF.CMake.Debug.sh
time ./BuildNF.CMake.Release.sh
cd ../../

cd _Out/
chmod 777 *.sh
cd  Tester/
chmod 777 *.sh
cd ../../

#pwd
