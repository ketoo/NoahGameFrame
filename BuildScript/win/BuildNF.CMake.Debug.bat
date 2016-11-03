cd Build.Win.Debug

cmake ../../../ -G "Visual Studio 14 2015 Win64" -DBUILD_TOOLS_ONLY=OFF -DCMAKE_BUILD_TYPE=Debug
cd ../