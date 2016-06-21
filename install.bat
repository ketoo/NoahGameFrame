cd Dependencies
call build_dep.bat

call BuildNF.CMake.Tools.bat
call BuildNF.Tools.VS2015.Debug.X64.bat

cd _Out\Server\NFDataCfg\tool\
call copy_files.bat 1
cd ..
cd ..
cd ..
cd ..

call BuildNF.CMake.bat
call BuildNF.VS2015.Debug.X64.bat

call BuildNF.CopyDll.bat
