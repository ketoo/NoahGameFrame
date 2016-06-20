# compile dep libraries
cd ./Dependencies
chmod -R 755 build_dep.sh
./build_dep.sh

cd ../

./BuildNF.CMake.sh

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
