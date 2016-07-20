cd Dependencies
call build_dep.bat
cd ..
cd _Out\Server\NFDataCfg\Tool\
start NFFileProcess.exe
cd ..
call GenerateConfigXML.bat
