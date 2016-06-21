cd Dependencies
call build_dep.bat

cd _Out\Server\NFDataCfg\tool\
call copy_files.bat 1
cd ..
cd ..
cd ..
cd ..

call BuildNF.CMake.bat
call BuildNF.VS2015.Debug.X64.bat
