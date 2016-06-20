# compile dep libraries
cd ./Dependencies
chmod -R 755 build_dep.sh
./build_dep.sh

cd ../

cd ./cmake.linux/
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
make

cd ..
cd _Out/Server/NFDataCfg/Tool/
./NFFileProcess
./NFAutoCopyDll

cd ..
cd ..
cd ..
cd ..
pwd

# generate makefile
#chmod -R 755 Build4Makefile.sh
#./Build4Makefile.sh

#pwd
