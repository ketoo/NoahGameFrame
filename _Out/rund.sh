#!/bin/bash

export LC_ALL="C"

ulimit -c unlimited
source /etc/profile


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


cd Debug

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d -d Plugin.xml Server=MasterServer ID=3

sleep 1

./NFPluginLoader_d -d Plugin.xml Server=WorldServer ID=7

sleep 1

./NFPluginLoader_d -d Plugin.xml Server=DBServer ID=8

sleep 1

./NFPluginLoader_d -d Plugin.xml Server=LoginServer ID=4

sleep 1

./NFPluginLoader_d -d Plugin.xml Server=GameServer ID=6

sleep 1

./NFPluginLoader_d -d Plugin.xml Server=ProxyServer ID=5

sleep 5

ps -A|grep NF
