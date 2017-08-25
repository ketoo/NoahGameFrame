echo Building dependencies...

mkdir -p lib/Debug/
mkdir -p lib/Release/

# compiling libevent
cd libevent
chmod +x ./configure
./configure CPPFLAGS=-fPIC --disable-shared --disable-openssl
make

cp -R -f ./.libs/*.a ../lib/Debug/
cp -R -f ./.libs/*.a ../lib/
cp -R -f ./.libs/*.a ../lib/Release/
cd ../

# compiling protobuf
cd protobuf
#chmod -R 755 *
./configure CXXFLAGS=-fPIC
make
make check

cp -R -f ./src/.libs/*.a ../lib/Debug/
cp -R -f ./src/.libs/*.a ../lib/
cp -R -f ./src/.libs/*.a ../lib/Release/

cp -r -f ./src/.libs/*.so ../../_Out/Debug/
cp -r -f ./src/.libs/*.so.* ../../_Out/Debug/
cp -r -f ./src/.libs/*.so ../../_Out/Release/
cp -r -f ./src/.libs/*.so.* ../../_Out/Release/

cp -r -f ./src/.libs/*.dylib ../../_Out/Debug/
cp -r -f ./src/.libs/*.dylib.* ../../_Out/Debug/
cp -r -f ./src/.libs/*.dylib ../../_Out/Release/
cp -r -f ./src/.libs/*.dylib.* ../../_Out/Release/

cp -r -f ./src/.libs/*.a ../../_Out/Debug/
cp -r -f ./src/.libs/*.a ../../_Out/Release/

cd ../

# compiling Theron
cd Theron
#chmod -R 755 *
make library mode=debug boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/libtherond.a ../lib/Debug/
cp -r -f ./Lib/libtherond.a ../lib/
make clean
make library mode=release boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/libtheron.a ../lib/Release/
cp -r -f ./Lib/libtheron.a ../lib/
make clean
cd ../

#mysql
# extract mysql libs
echo Extract mysql libs
unzip -o mysql_libs.zip -d ./

echo Extract mysql++ libs
unzip -o mysqlpp_libs.zip -d ./
# copy libs
cp -R -f ./lib/Release/libmysqlclient.so ../_Out/Debug/
cp -R -f ./lib/Release/libmysqlclient.so ../_Out/Release/

cp -R -f ./lib/Release/libmysqlclient.a ./lib/Debug/
cp -R -f ./lib/Release/libmysqlpp.a ./lib/Debug/

cp -R -f ./lib/Release/libmysqlclient.so ./lib/
cp -R -f ./lib/Release/libmysqlclient.so ./lib/

cp -R -f ./lib/Release/libmysqlclient.a ./lib/
cp -R -f ./lib/Release/libmysqlpp.a ./lib/

# TODO: other libs
unzip -o gperftools-2.5.zip -d ./
cd gperftools-2.5
#chmod -R 755 *
./configure --enable-frame-pointers
make
make install
cd ../


# back to main dir
pwd
