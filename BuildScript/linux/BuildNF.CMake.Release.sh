mkdir Build.Linux.Release

cd Build.Linux.Release

cmake ../../../ -G "CodeBlocks - Unix Makefiles" -DBUILD_MID_WARE_SDK=OFF -DCMAKE_BUILD_TYPE=Release

make
cd ../
