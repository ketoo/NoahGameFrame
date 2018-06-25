echo Building dependencies...

#sudo apt-get install automake
#sudo apt-get install zip unzip

mkdir -p lib/Debug/
mkdir -p lib/Release/

# compiling libevent
cd libevent
chmod +x ./configure
./configure CPPFLAGS=-fPIC --disable-shared --disable-openssl
make

cp -R -f ./.libs/*.a ../lib/Debug/
cp -R -f ./.libs/*.a ../lib/Release/

cd ../


# compiling protobuf
cd protobuf
chmod -R 755 *
./configure CXXFLAGS=-fPIC
make
make check

cp -r -f ./src/.libs/*.a ../lib/Debug/
cp -r -f ./src/.libs/*.a ../lib/Release/

cp -r -f ./src/.libs/*.so ../../_Out/Debug/
cp -r -f ./src/.libs/*.so.* ../../_Out/Debug/
cp -r -f ./src/.libs/*.so ../../_Out/Release/
cp -r -f ./src/.libs/*.so.* ../../_Out/Release/

cp -r -f ./src/.libs/*.dylib ../../_Out/Debug/
cp -r -f ./src/.libs/*.dylib.* ../../_Out/Debug/
cp -r -f ./src/.libs/*.dylib ../../_Out/Release/
cp -r -f ./src/.libs/*.dylib.* ../../_Out/Release/

cd ../

# compiling Theron
cd Theron
chmod -R 755 *
make library mode=debug boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/*.a ../lib/Debug/

make clean
make library mode=release boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/*.a ../lib/Release/
make clean
cd ../

# compiling lua
echo Building lua...
cd lua

sysOS=`uname -s`
if [ $sysOS == "Darwin" ];then
    make macosx
elif [ $sysOS == "Linux" ];then
    make linux
fi

cp -r -f ./*.a ../lib/Debug/
cp -r -f ./*.a ../lib/Release/
cd ../
echo Building lua finish...

# compiling hiredis
echo Building hiredis...
chmod 777 build_hiredis.sh
./build_hiredis.sh
echo Building hiredis finish...




# TODO: other libs
#unzip -o gperftools-2.5.zip -d ./
#cd gperftools-2.5
#chmod -R 755 *
#./configure --enable-frame-pointers
#make && make install

#cp -R -f ./.libs/*.a ../lib/Debug/
#cp -R -f ./.libs/*.a ../lib/Release/

#cp -r -f ./.libs/*.so ../../_Out/Debug/
#cp -r -f ./.libs/*.so.* ../../_Out/Debug/
#cp -r -f ./.libs/*.so ../../_Out/Release/
#cp -r -f ./.libs/*.so.* ../../_Out/Release/

#cp -r -f ./.libs/*.dylib ../../_Out/Debug/
#cp -r -f ./.libs/*.dylib.* ../../_Out/Debug/
#cp -r -f ./.libs/*.dylib ../../_Out/Release/
#cp -r -f ./.libs/*.dylib.* ../../_Out/Release/
#cd ../

#-ltcmalloc
#-lprofiler

# back to main dir
pwd
