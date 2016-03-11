cp -a ../../Comm/Release/NFPluginLoader_r ./NFMasterServer/NFMasterServer_r
cp -a ../../Comm/Release/NFPluginLoader_r ./NFWorldServer/NFWorldServer_r
cp -a ../../Comm/Release/NFPluginLoader_r ./NFLoginServer/NFLoginServer_r
cp -a ../../Comm/Release/NFPluginLoader_r ./NFGameServer1/NFGameServer_r
cp -a ../../Comm/Release/NFPluginLoader_r ./NFProxyServer1/NFProxyServer_r

export LC_ALL="C"

cd ./NFMasterServer
chmod -R 777  NFMasterServer_r
./NFMasterServer_r -d
cd ../

cd ./NFWorldServer
chmod -R 777  NFWorldServer_r
./NFWorldServer_r -d
cd ../


cd ./NFLoginServer
chmod -R 777  NFLoginServer_r
./NFLoginServer_r -d
cd ../



cd ./NFGameServer1
chmod -R 777  NFGameServer_r
./NFGameServer_r -d
cd ../


cd ./NFProxyServer1
chmod -R 777  NFProxyServer_r
./NFProxyServer_r -d
cd ../




ps -A|grep NF
