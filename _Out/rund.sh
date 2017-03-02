export LC_ALL="C"

cd Debug

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d -d Server=MasterServer ID=3

./NFPluginLoader_d -d Server=WorldServer ID=7

./NFPluginLoader_d -d Server=LoginServer ID=4

./NFPluginLoader_d -d Server=GameServer ID=6

./NFPluginLoader_d -d Server=ProxyServer ID=5

ps -A|grep NF
