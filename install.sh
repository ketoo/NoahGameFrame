# compile dep libraries
cd ./Dependencies
chmod -R 755 build_dep.sh
./build_dep.sh
cd ../

cd BuildScript
chmod -R 755 ./BuildNF.CMake.Tools.sh
chmod -R 755 ./BuildNF.Tools.Linux.Debug.sh
./BuildNF.CMake.Tools.sh
./BuildNF.Tools.Linux.Debug.sh
cd ..

cd _Out/Server/NFDataCfg/Tool/
chmod 755 ./NFFileProcess
chmod 755 ./copy_files.sh
./copy_files.sh
cd ..
cd ..
cd ..
cd ..

cd BuildScript
chmod -R 755 ./BuildNF.CMake.Debug.sh
chmod -R 755 ./BuildNF.CMake.Release.sh
chmod -R 755 ./BuildNF.CopyDll.sh
./BuildNF.CMake.Debug.sh
./BuildNF.CopyDll.sh
cd ..

cd _Out/Server/Debug/
chmod -R 755 ./rund_cmake.sh
chmod -R 755 ./killd_cmake.sh
cd ..
cd ..
cd ..

cd _Out/Server/Release/
chmod -R 755 ./run_cmake.sh
chmod -R 755 ./kill_cmake.sh
cd ..
cd ..
cd ..

# generate makefile
#chmod -R 755 Build4Makefile.sh
#./Build4Makefile.sh

#pwd
