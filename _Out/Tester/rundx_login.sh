export LC_ALL="C"
export DYLD_LIBRARY_PATH=.:$DYLD_LIBRARY_PATH

cd ../Debug/

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d Server=LoginServer ID=4

ps -A|grep NF
