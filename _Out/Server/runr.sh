export LC_ALL="C"

cd Release

chmod -R 777  NFPluginLoader_r

./NFPluginLoader_r -d Server=MasterServer ID=3

./NFPluginLoader_r -d Server=WorldServer ID=7

./NFPluginLoader_r -d Server=LoginServer ID=4

./NFPluginLoader_r -d Server=GameServer ID=6

./NFPluginLoader_r -d Server=ProxyServer ID=5

ps -A|grep NF
