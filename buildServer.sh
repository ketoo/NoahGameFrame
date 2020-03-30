# make sure cmake is installed
# sudo apt-get update

#g++  sudo apt-get install g++
#unzip sudo apt-get install unzip

#compile dep libraries
rm -rf ./_Out/Debug/NFServer
rm -rf ./_Out/Debug/*.a

rm -rf ./_Out/Release/NFServer
rm -rf ./_Out/Release/*.a

if [ "$1" == "BUILD_MID_WARE" ]; then
   echo "we dont need to compile message and tools again"
else
   echo "we only build NF SDK here"
    cd NFComm/NFMessageDefine
    ./cpp.sh
    cd ../../

    cd BuildScript/linux/
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
fi


cd BuildScript/linux/
chmod -R 755 ./BuildNF.CMake.Debug.sh
chmod -R 755 ./BuildNF.CMake.Release.sh

#time ./BuildNF.CMake.Debug.sh  BUILD_MID_WARE
time ./BuildNF.CMake.Debug.sh  $1
time ./BuildNF.CMake.Release.sh  $1
cd ../../

cd _Out/
chmod 777 *.sh
cd ../

#pwd
