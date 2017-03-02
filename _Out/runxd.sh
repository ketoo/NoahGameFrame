export LC_ALL="C"

cd Debug

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d -d PluginX.xml Server=MasterServer ID=3

./NFPluginLoader_d -d PluginX.xml Server=WorldServer ID=7

./NFPluginLoader_d -d PluginX.xml Server=LoginServer ID=4

./NFPluginLoader_d -d PluginX.xml Server=GameServer ID=6

./NFPluginLoader_d -d PluginX.xml Server=ProxyServer ID=5

ps -A|grep NF
