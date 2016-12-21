@echo off 

rd /q /s _Out
rd /q /s BuildScript
rd /q /s Introduce
rd /q /s Dependencies
rd /q /s NFTools
rd /q /s Tool
rd /q /s Tutorial
rd /q /s NFComm\Config
rd /q /s NFComm\NFMessageDefine


del NFComm\NFLogPlugin\easylogging++.h
del NFComm\NFConfigPlugin\myrc4.h
del NFComm\NFConfigPlugin\myrc4.cpp
echo. & pause 