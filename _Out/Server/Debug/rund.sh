cp -a ../../Comm/Debug/NFPluginLoader_d ./NFMasterServer/NFMasterServer_d
cp -a ../../Comm/Debug/NFPluginLoader_d ./NFWorldServer/NFWorldServer_d
cp -a ../../Comm/Debug/NFPluginLoader_d ./NFLoginServer/NFLoginServer_d
cp -a ../../Comm/Debug/NFPluginLoader_d ./NFGameServer1/NFGameServer_d
cp -a ../../Comm/Debug/NFPluginLoader_d ./NFProxyServer1/NFProxyServer_d

export LC_ALL="C"

cd ./NFMasterServer
chmod -R 777  NFMasterServer_d
./NFMasterServer_d -d
cd ../

cd ./NFWorldServer
chmod -R 777  NFWorldServer_d
./NFWorldServer_d -d
cd ../


cd ./NFLoginServer
chmod -R 777  NFLoginServer_d
./NFLoginServer_d -d
cd ../



cd ./NFGameServer1
chmod -R 777  NFGameServer_d
./NFGameServer_d -d
cd ../


cd ./NFProxyServer1
chmod -R 777  NFProxyServer_d
./NFProxyServer_d -d
cd ../




ps -A|grep NF
