export LC_ALL="C"

cd ../Debug/

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d Server=GameServer ID=6

ps -A|grep NF
