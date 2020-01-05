cd Build.Linux.Debug

if [ "$1" == 'BUILD_MID_WARE' ] ; then
cmake ../../../ -G "CodeBlocks - Unix Makefiles"  -DBUILD_MID_WARE_SDK=ON -DCMAKE_BUILD_TYPE=Debug
else
cmake ../../../ -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
fi

make
cd ../
