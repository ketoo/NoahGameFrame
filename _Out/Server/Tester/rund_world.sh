export LC_ALL="C"

cd Debug

chmod -R 777  NFPluginLoader_d

../Debug/NFPluginLoader_d Server=WorldServer ID=7

ps -A|grep NF
