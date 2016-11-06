cd /d %~dp0

@echo off

taskkill /f /im MasterSvr*
taskkill /f /im LoginSvr*
taskkill /f /im ProxySvr*
taskkill /f /im GameSvr*
taskkill /f /im WorldSvr*



