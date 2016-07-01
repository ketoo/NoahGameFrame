export LC_ALL="C"

echo "start NFMasterServer"
cd ./NFMasterServer
chmod -R 777  NFPluginLoader_d
./NFPluginLoader_d -d
cd ../

sleep 5s

echo "start NFWorldServer"
cd ./NFWorldServer
chmod -R 777  NFPluginLoader_d
./NFPluginLoader_d -d
cd ../

sleep 5s

echo "start NFLoginServer"
cd ./NFLoginServer
chmod -R 777  NFPluginLoader_d
./NFPluginLoader_d -d
cd ../

sleep 5s

echo "start NFGameServer1"
cd ./NFGameServer1
chmod -R 777  NFPluginLoader_d
./NFPluginLoader_d -d
cd ../

sleep 5s

echo "start NFProxyServer1"
cd ./NFProxyServer1
chmod -R 777  NFPluginLoader_d
./NFPluginLoader_d -d
cd ../




ps -A|grep NF
