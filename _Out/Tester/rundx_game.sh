export LC_ALL="C"

sysOS=`uname -s`
if [ $sysOS == "Darwin" ];then
    echo "I'm MacOS"
    export DYLD_LIBRARY_PATH=.:$DYLD_LIBRARY_PATH
elif [ $sysOS == "Linux" ];then
    echo "I'm Linux"
    export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
else
    echo "Other OS: $sysOS"
fi

cd ../Debug/

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d Plugin.xml Server=GameServer ID=6

ps -A|grep NF
