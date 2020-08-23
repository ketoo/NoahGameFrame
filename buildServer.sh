# make sure cmake is installed
# sudo apt-get update

#g++  sudo apt-get install g++
#unzip sudo apt-get install unzip

#compile dep libraries

#example 1: ./buildServer
#example 2: ./buildServer BUILD_MID_WARE DEBUG
#example 3: ./buildServer BUILD_MID_WARE

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

if [ "$2" == "DEBUG" ]; then
rm -rf ./_Out/Debug/NFServer
rm -rf ./_Out/Debug/*.a

cd BuildScript/linux/
chmod -R 755 ./BuildNF.CMake.Debug.sh
time ./BuildNF.CMake.Debug.sh  $1

else

rm -rf ./_Out/Release/NFServer
rm -rf ./_Out/Release/*.a

cd BuildScript/linux/
chmod -R 755 ./BuildNF.CMake.Release.sh
time ./BuildNF.CMake.Release.sh  $1
fi

cd ../../

cd _Out/
chmod 777 *.sh
cd ../

#pwd
