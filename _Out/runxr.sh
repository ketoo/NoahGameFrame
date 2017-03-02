export LC_ALL="C"

cd Release

chmod -R 777  NFPluginLoader

./NFPluginLoader -d PluginX.xml Server=MasterServer ID=3

./NFPluginLoader -d PluginX.xml Server=WorldServer ID=7

./NFPluginLoader -d PluginX.xml Server=LoginServer ID=4

./NFPluginLoader -d PluginX.xml Server=GameServer ID=6

./NFPluginLoader -d PluginX.xml Server=ProxyServer ID=5

ps -A|grep NF
