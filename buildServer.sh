# make sure cmake is installed
# sudo apt-get update

#g++  sudo apt-get install g++
#unzip sudo apt-get install unzip

#compile dep libraries
rm -rf ./_Out/Debug
rm -rf ./_Out/Release

cd NFComm/NFMessageDefine
./cpp.sh
cd ../../

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
#time ./BuildNF.CMake.Release.sh
cd ../../

cd _Out/
chmod 777 *.sh
cd  Tester/
chmod 777 *.sh
cd ../../

#pwd
