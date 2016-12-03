export LC_ALL="C"

cd ../Debug/

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d Server=LoginServer ID=4

ps -A|grep NF
