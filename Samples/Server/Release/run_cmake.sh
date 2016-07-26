export LC_ALL="C"

cd ./NFMasterServer
chmod -R 777  NFPluginLoader
./NFPluginLoader -d
cd ../

cd ./NFWorldServer
chmod -R 777  NFPluginLoader
./NFPluginLoader -d
cd ../


cd ./NFLoginServer
chmod -R 777  NFPluginLoader
./NFPluginLoader -d
cd ../



cd ./NFGameServer1
chmod -R 777  NFPluginLoader
./NFPluginLoader -d
cd ../


cd ./NFProxyServer1
chmod -R 777  NFPluginLoader
./NFPluginLoader -d
cd ../




ps -A|grep NF
