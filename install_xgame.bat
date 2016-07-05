setlocal enabledelayedexpansion
echo off
set isCmake=0
cmake --version | findstr /i /c:"CMake suite maintained and supported by" && set isCmake=1 || goto wait

:wait
If %isCmake%==0 echo Please install CMake and put CMake to your Path! & pause & exit 1
echo on

xcopy cmake.dir\x-game\*.* *.txt /e /y
cd Dependencies
call build_dep.bat

cd BuildScript
call BuildNF.CMake.Tools.bat
call BuildNF.Tools.VS2015.Debug.X64.bat
cd ..
 
cd _Out\Server\NFDataCfg\tool\
call copy_files.bat 1
cd ..
cd ..
cd ..
cd ..
 
cd BuildScript
call BuildNF.CMake.Debug.bat
call BuildNF.VS2015.Debug.X64.bat
 
call BuildNF.CopyDll.bat
cd ..
pause