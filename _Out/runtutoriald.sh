#!/bin/bash

export LC_ALL="C"

cd Debug

chmod -R 777  NFPluginLoader_d

./NFPluginLoader_d Server=TutorialServer ID=5 Tutorial5.xml

ps -A|grep NF
