CLS
@echo off

color 0a

Title =====Extract NoahFramework 3rd party libs(Windows)======

rem ========================================================================
echo Start to extract boost_1_53_0

cd Tool
7z x "..\Dependencies\boost_1_53_0.zip.001" -o"..\Dependencies\" -y

echo Extract boost_1_53_0 completed!
pause

rem ========================================================================

echo Start to extract boost_1_53_0 VS2010 VS2012 libs

7z x "..\Dependencies\boost_1_53_0\stage\lib\x64\vs10_0.zip" -o"..\Dependencies\boost_1_53_0\stage\lib\x64\" -y
7z x "..\Dependencies\boost_1_53_0\stage\lib\x64\vs11_0.zip" -o"..\Dependencies\boost_1_53_0\stage\lib\x64\" -y

echo Extract boost_1_53_0 VS2010 VS2012 libs completed!
pause

rem ========================================================================

rem to add more