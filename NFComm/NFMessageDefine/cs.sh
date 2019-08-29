
./protoc --csharp_out="./" --proto_path="./" NFDefine.proto
./protoc --csharp_out="./" --proto_path="./" NFMsgBase.proto
./protoc --csharp_out="./" --proto_path="./" NFMsgPreGame.proto
./protoc --csharp_out="./" --proto_path="./" NFMsgShare.proto
./protoc --csharp_out="./" --proto_path="./" NFMsgExtra.proto
./protoc --csharp_out="./" --proto_path="./" NFMsgFriend.proto

cp -a *.cs ../../_Out/NFDataCfg/client/proto/
