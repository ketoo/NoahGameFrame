cd Dependencies
call build_dep.bat

call BuildNF.CMake.bat
cd ..
call BuildNF.VS2015.Debug.X64.bat
cd ..

cd _Out\Server\NFDataCfg\tool\
start NFAutoCopyDll.exe
start NFFileProcess.exe