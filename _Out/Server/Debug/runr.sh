export LC_ALL="C"

cd ./NFMasterServer
chmod -R 777  NFPluginLoader_r 
./NFPluginLoader_r &
cd ../

cd ./NFWorldServer
chmod -R 777  NFPluginLoader_r 
./NFPluginLoader_r &
cd ../


cd ./NFLoginServer
chmod -R 777  NFPluginLoader_r 
./NFPluginLoader_r &
cd ../



cd ./NFGameServer1
chmod -R 777  NFPluginLoader_r 
./NFPluginLoader_r &
cd ../


cd ./NFProxyServer1
chmod -R 777  NFPluginLoader_r 
./NFPluginLoader_r &
cd ../




ps -A|grep NF