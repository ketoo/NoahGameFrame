# make sure cmake is installed
# sudo apt-get update

#g++  sudo apt-get install g++
#unzip sudo apt-get install unzip

#compile dep libraries

#example 1: ./buildServer.sh
#example 2: ./buildServer.sh BUILD_MID_WARE DEBUG
#example 3: ./buildServer.sh BUILD_MID_WARE RELEASE
#example 4: ./buildServer.sh BUILD_TOOLS_ONLY

arg_1="BUILD_MID_WARE"
arg_2="RELEASE"

echo "$1"
echo "$2"

if [ "$#" == 1 ]; then
   echo "args == 1"
   arg_1="$1"
fi
if [ "$#" == 2 ]; then
   echo "args == 2"
   arg_1=$1
   arg_2=$2
fi

echo "$arg_1"
echo "$arg_2"

if [ "$arg_1" == "BUILD_MID_WARE" ]; then
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

if [ "$#" == 2 ] || [ "$#" == 0 ]; then

if [ "$arg_2" == "DEBUG" ]; then
   echo "build NF DEBUG"
rm -rf ./_Out/Debug/NFServer
rm -rf ./_Out/Debug/*.a

cd BuildScript/linux/
chmod -R 755 ./BuildNF.CMake.Debug.sh
time ./BuildNF.CMake.Debug.sh  $arg_1

elif [ "$arg_2" == "RELEASE" ]; then
   echo "build NF RELEASE"

rm -rf ./_Out/Release/NFServer
rm -rf ./_Out/Release/*.a

cd BuildScript/linux/
chmod -R 755 ./BuildNF.CMake.Release.sh
time ./BuildNF.CMake.Release.sh  $arg_1
fi

fi

cd ../../

cd _Out/
chmod 777 *.sh
cd ../

#pwd
