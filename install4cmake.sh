# make sure cmake is installed
cmake --version
if [ $? -ne 0 ]; then
    echo "[ERROR] Please install cmake first."
    echo "[ubuntu] apt-get install cmake or [centos] yum install cmake or [mac] brew install cmake"
    exit 1
fi


#compile dep libraries
cd ./Dependencies
chmod -R 755 build_dep.sh
./build_dep.sh
cd ../

cd BuildScript/linux/
chmod -R 755 ./BuildNF.CMake.Tools.sh
chmod -R 755 ./BuildNF.Tools.Linux.Debug.sh
./BuildNF.CMake.Tools.sh
./BuildNF.Tools.Linux.Debug.sh
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
./BuildNF.CMake.Debug.sh
./BuildNF.CMake.Release.sh
cd ../../

cd _Out/
chmod 777 *.sh
cd  Tester/
chmod 777 *.sh

cd ../../
# generate makefile
#chmod -R 755 Build4Makefile.sh
#./Build4Makefile.sh

#pwd