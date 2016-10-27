export LC_ALL="C"

chmod -R 777  NFPluginLoader_r

./NFPluginLoader_r -d Server=MasterServer ID=1

./NFPluginLoader_r -d Server=NFWorldServer ID=2

./NFPluginLoader_r -d Server=NFLoginServer ID=3

./NFPluginLoader_r -d Server=NFGameServer ID=4

./NFPluginLoader_r -d Server=NFProxyServer ID=5

ps -A|grep NF
