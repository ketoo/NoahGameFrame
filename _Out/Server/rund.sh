export LC_ALL="C"

cd Debug

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d -d Server=MasterServer ID=3

./NFPluginLoader_d -d Server=NFWorldServer ID=7

./NFPluginLoader_d -d Server=NFLoginServer ID=4

./NFPluginLoader_d -d Server=NFGameServer ID=6

./NFPluginLoader_d -d Server=NFProxyServer ID=5

ps -A|grep NF
