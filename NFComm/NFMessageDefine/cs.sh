chmod 777 protoc

./protoc --csharp_out="./" --proto_path="./" NFDefine.proto
./protoc --csharp_out="./" --proto_path="./" NFMsgBase.proto
./protoc --csharp_out="./" --proto_path="./" NFMsgPreGame.proto
./protoc --csharp_out="./" --proto_path="./" NFMsgShare.proto

cp -a *.cs ../../_Out/NFDataCfg/client/proto/
