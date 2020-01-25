mkdir Build.Linux.Release

cd Build.Linux.Release

if [ "$1" == 'BUILD_MID_WARE' ] ; then
cmake ../../../ -G "CodeBlocks - Unix Makefiles"  -DBUILD_MID_WARE_SDK=ON -DCMAKE_BUILD_TYPE=Release
else
cmake ../../../ -G "CodeBlocks - Unix Makefiles" -DBUILD_MID_WARE_SDK=OFF -DCMAKE_BUILD_TYPE=Release
fi

make
cd ../
