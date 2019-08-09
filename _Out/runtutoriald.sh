#!/bin/bash

export LC_ALL="C"

cd Release

chmod -R 777  NFPluginLoader

./NFPluginLoader Server=TutorialServer ID=4 Tutorial4.xml

ps -A|grep NF
