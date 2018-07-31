#!/bin/bash

ps -A|grep NF

sysOS=`uname -s`
if [ $sysOS == "Darwin" ];then
	echo "I'm MacOS"
elif [ $sysOS == "Linux" ];then
	echo "I'm Linux"
else
	echo "Other OS: $sysOS"
fi

#pgrep NF | xargs kill -s 9
kill -9 $(ps -ef|grep NF |awk '$0 !~/grep/ {print $2}' |tr -s '\n' ' ')

sleep 3

ps -A|grep NF