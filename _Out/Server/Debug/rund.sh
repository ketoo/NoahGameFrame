export LC_ALL="C"

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d -d Server=MasterServer ID=1

./NFPluginLoader_d -d Server=NFWorldServer ID=2

./NFPluginLoader_d -d Server=NFLoginServer ID=3

./NFPluginLoader_d -d Server=NFGameServer ID=4

./NFPluginLoader_d -d Server=NFProxyServer ID=5

ps -A|grep NF
