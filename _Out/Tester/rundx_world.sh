export LC_ALL="C"
export DYLD_LIBRARY_PATH=.:$DYLD_LIBRARY_PATH

cd ../Debug/

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d PluginX.xml Server=WorldServer ID=7

ps -A|grep NF
