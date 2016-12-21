export LC_ALL="C"

cd ../Debug/

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d Server=MasterServer ID=3

ps -A|grep NF
