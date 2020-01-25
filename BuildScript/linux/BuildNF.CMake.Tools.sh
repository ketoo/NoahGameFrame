mkdir Build.Tools

cd Build.Tools

cmake ../../../ -G "CodeBlocks - Unix Makefiles" -DBUILD_TOOLS_ONLY=ON -DCMAKE_BUILD_TYPE=Debug
make
cd ../