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