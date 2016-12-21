
cd Dependencies
call build_dep.bat
cd ..

cd BuildScript\win\
call BuildNF.CMake.Tools.bat
call BuildNF.Tools.VS2015.Debug.X64.bat
call BuildNF.Tools.VS2015.Release.X64.bat
cd ..\..\

cd _Out\NFDataCfg\tool\

call copy_files.bat 1
cd ..\..\..\

cd BuildScript\win\
call BuildNF.CMake.Debug.bat
call BuildNF.VS2015.Debug.X64.bat
call BuildNF.CMake.Release.bat
call BuildNF.VS2015.Release.X64.bat

cd ..\..\

pause