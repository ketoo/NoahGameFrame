rem �����ߣ���־�� great3779@sina.com 20130403


rem @echo off


rem �ȱ���bjam
start bootstrap.bat


rem �ȴ�һ���Ӵ�bjam������ɣ�����������ܹ���������õȴ�ʱ�����һЩ��http://anders0913.iteye.com/blog/375303
SET SLEEP=ping 127.0.0.1 -n
%SLEEP% 60 > nul


rem ����bjam����boost��
rem �ڴˣ����Ǳ���vs2008��vs2010�µ�x86��x64��4��boost���ļ�


rem �������ļ���

rem vs2010 win32���ļ���
md stage\lib\win32\vs10_0\


rem vs2010 x64���ļ���
md stage\lib\x64\vs10_0\


rem ******************************************************************
rem ��ɾ��lib�������ļ�����ɾ���ļ��У�
del stage\lib\*.* /Q


rem ����vs2010 win32���ļ�
bjam --toolset=msvc-10.0 architecture=x86 link=static


rem ������\stage\lib\win32\vs10_0
copy stage\lib\*.* stage\lib\win32\vs10_0\*.* /Y
rem ##################################################################


rem ******************************************************************
rem ��ɾ��lib�������ļ�����ɾ���ļ��У�
del stage\lib\*.* /Q


rem ����vs2010 x64���ļ�
bjam --toolset=msvc-10.0 architecture=x86 address-model=64 link=static


rem ������\stage\lib\x64\vs10_0
copy stage\lib\*.* stage\lib\x64\vs10_0\*.* /Y
rem ##################################################################


rem ɾ��lib�������ļ�����ɾ���ļ��У�
del stage\lib\*.* /Q