./NFFileProcess
cp -a ../proto/NFProtocolDefine.hpp ../../../NFComm/NFMessageDefine

rm -rf ../client
mkdir ../client
mkdir ../client/Ini
mkdir ../client/proto
mkdir ../client/Struct

cp -a ../Ini ../client/
cp -a ../proto ../client/
cp -a ../Struct ../client/


cp -a ../../../NFComm/NFMessageDefine/*.cs ../client/proto/
