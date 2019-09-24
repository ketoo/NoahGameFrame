echo Building dependencies...

git submodule update --init --recursive

sudo apt-get install automake
sudo apt-get install zip unzip

rm -rf ./lib
mkdir -p lib/Debug/
mkdir -p lib/Release/

sudo apt-get install libssl-dev
sudo apt-get install libreadline-dev 
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

cp -R -f ./.libs/*.a ../lib/Release/

cd ../


# compiling protobuf
cd protobuf
make clean
chmod -R 755 *
./configure --disable-shared --with-protoc
make
make check

cp -r -f ./src/.libs/*.a ../lib/Debug/
cp -r -f ./src/.libs/*.a ../lib/Release/

cp -r -f ./src/protoc ../../NFComm/NFMessageDefine/protoc

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
chmod 777 build_hiredis.sh
./build_hiredis.sh
echo Building hiredis finish...




# TODO: other libs
#unzip -o gperftools-2.7.zip -d ./
#cd gperftools-2.7
#chmod -R 755 *
#./configure --enable-frame-pointers --disable-heap-profiler --disable-heap-checker
#make

#cp -R -f ./.libs/*.a ../lib/Debug/
#cp -R -f ./.libs/*.a ../lib/Release/


#-ltcmalloc
#-lprofiler

# back to main dir
pwd
