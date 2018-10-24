echo Building dependencies...

sysOS=`uname -s`

rm -rf ./lib
mkdir -p lib/Debug/
mkdir -p lib/Release/

if [ $sysOS == "Darwin" ];then
    brew install libtool
    brew install automake
    brew install zip unzip

elif [ $sysOS == "Linux" ];then
    apt-get install automake
    apt-get install zip unzip

	apt-get install libreadline6-dev 
	apt-get install libncurses5-dev
fi

#cd openssl-1.1.0h
#rm -rf *.a
#chmod -R 755 *
#./config no-shared
#make depend
#make
#cp -r -f ./*.a ../lib/Debug/
#cp -r -f ./*.a ../lib/Release/
#cd ../

# compiling libevent
cd libevent
make clean
chmod +x ./configure
#./configure --disable-shared --disable-openssl
./configure --disable-shared --disable-openssl
make

cp -R -f ./.libs/*.a ../lib/Debug/
cp -R -f ./.libs/*.a ../lib/Release/

cd ../


# compiling protobuf
cd protobuf
make clean
chmod -R 755 *
./configure --disable-shared
make
make check

cp -r -f ./src/.libs/*.a ../lib/Debug/
cp -r -f ./src/.libs/*.a ../lib/Release/

cd ../

# compiling Theron
cd Theron
chmod -R 755 *
make library mode=debug boost=off c++11=on posix=on shared=off
cp -r -f ./Lib/*.a ../lib/Debug/

make clean
make library mode=release boost=off c++11=on posix=on shared=off
cp -r -f ./Lib/*.a ../lib/Release/
make clean
cd ../

# compiling lua
echo Building lua...
cd lua

if [ $sysOS == "Darwin" ];then
    make macosx test
elif [ $sysOS == "Linux" ];then

    make linux test
fi

cp -r -f ./src/*.a ../lib/Debug/
cp -r -f ./src/*.a ../lib/Release/
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
