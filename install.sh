# compile dep libraries
cd ./Dependencies
chmod -R 755 build_dep.sh
./build_dep.sh
cd ../

cd _Out/Server/NFDataCfg/Tool/
chmod 755 ./NFFileProcess
./NFFileProcess
cd ..
cd ..
cd ..
cd ..

./BuildNF.CMake.sh
./BuildNF.CopyDll.sh

# generate makefile
#chmod -R 755 Build4Makefile.sh
#./Build4Makefile.sh

#pwd
