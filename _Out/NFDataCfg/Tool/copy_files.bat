NFFileProcess.exe %1
copy ..\\proto\\NFProtocolDefine.hpp ..\\..\\..\\NFComm\\NFMessageDefine\\ /Y

rd /s /q ..\\client
md ..\\client
md ..\\client\\Ini
md ..\\client\\proto
md ..\\client\\Struct

copy ..\\Ini ..\\client\\Ini\\ /Y
copy ..\\proto ..\\client\\proto\\ /Y
copy ..\\Struct ..\\client\\Struct\\ /Y


copy ..\\..\\..\\NFComm\\NFMessageDefine\\*.cs ..\\client\\proto\\ /Y
