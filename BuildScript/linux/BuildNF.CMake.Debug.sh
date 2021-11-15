mkdir Build.Linux.Debug

cd Build.Linux.Debug

cmake ../../../ -G "CodeBlocks - Unix Makefiles" -DBUILD_MID_WARE_SDK=OFF -DCMAKE_BUILD_TYPE=Debug

make
cd ../
