echo Building dependencies...

sudo apt-get install automake
sudo apt-get install zip unzip
sudo apt-get install libtool

rm -rf ./lib
mkdir -p lib/Debug/
mkdir -p lib/Release/

sudo apt-get install libssl-dev
sudo apt-get install libreadline-dev 
cd openssl-1.1.0h
rm -rf *.a
chmod -R 755 *
./config no-shared
make depend
make
cp -r -f ./*.a ../lib/Debug/
cp -r -f ./*.a ../lib/Release/
cd ../

# compiling libevent
cd libevent
make clean
sh autogen.sh
chmod +x ./configure
./configure --disable-shared
make

cp -R -f ./.libs/*.a ../lib/Debug/
cp -R -f ./.libs/*.a ../lib/Release/

cd ../


# compiling protobuf
cd protobuf
make clean
chmod -R 755 *
sh autogen.sh
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

sysOS=`uname -s`
if [ $sysOS == "Darwin" ];then
    make macosx test
elif [ $sysOS == "Linux" ];then
	sudo apt-get install libreadline6-dev 
	sudo apt-get install libncurses5-dev
    make linux test
fi

cp -r -f ./src/*.a ../lib/Debug/
cp -r -f ./src/*.a ../lib/Release/
cd ../
echo Building lua finish...

# compiling hiredis
echo Building hiredis...
cd hiredis/hiredis_linux/hiredis
make
cp libhiredis.a ../../../lib/Debug/
cd ../../../
echo Building hiredis finish...

# compiling openssl
echo Building openssl
cd openssl
./config
make
cp *.a ../lib/Debug/
cd ../


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
