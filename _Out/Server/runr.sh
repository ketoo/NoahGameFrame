export LC_ALL="C"

cd Release

chmod -R 777  NFPluginLoader

./NFPluginLoader -d Server=MasterServer ID=3

./NFPluginLoader -d Server=WorldServer ID=7

./NFPluginLoader -d Server=LoginServer ID=4

./NFPluginLoader -d Server=GameServer ID=6

./NFPluginLoader -d Server=ProxyServer ID=5

ps -A|grep NF
