export LC_ALL="C"

cd Release

chmod -R 777  NFPluginLoader_r

./NFPluginLoader_r -d Server=MasterServer ID=3

./NFPluginLoader_r -d Server=NFWorldServer ID=7

./NFPluginLoader_r -d Server=NFLoginServer ID=4

./NFPluginLoader_r -d Server=NFGameServer ID=6

./NFPluginLoader_r -d Server=NFProxyServer ID=5

ps -A|grep NF
