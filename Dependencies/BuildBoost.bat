rem 创建者：黄志丹 great3779@sina.com 20130403


rem @echo off


rem 先编译bjam
start bootstrap.bat


rem 等待一分钟待bjam编译完成（如果电脑性能过差，可以设置等待时间更长一些）http://anders0913.iteye.com/blog/375303
SET SLEEP=ping 127.0.0.1 -n
%SLEEP% 60 > nul


rem 利用bjam编译boost库
rem 在此，我们编译vs2008和vs2010下的x86及x64共4类boost库文件


rem 建立库文件夹

rem vs2010 win32库文件夹
md stage\lib\win32\vs10_0\


rem vs2010 x64库文件夹
md stage\lib\x64\vs10_0\


rem ******************************************************************
rem 先删除lib下所有文件（不删除文件夹）
del stage\lib\*.* /Q


rem 编译vs2010 win32库文件
bjam --toolset=msvc-10.0 architecture=x86 link=static


rem 拷贝至\stage\lib\win32\vs10_0
copy stage\lib\*.* stage\lib\win32\vs10_0\*.* /Y
rem ##################################################################


rem ******************************************************************
rem 先删除lib下所有文件（不删除文件夹）
del stage\lib\*.* /Q


rem 编译vs2010 x64库文件
bjam --toolset=msvc-10.0 architecture=x86 address-model=64 link=static


rem 拷贝至\stage\lib\x64\vs10_0
copy stage\lib\*.* stage\lib\x64\vs10_0\*.* /Y
rem ##################################################################


rem 删除lib下所有文件（不删除文件夹）
del stage\lib\*.* /Q