echo on
protoc.exe --csharp_out="./" --proto_path="./" NFDefine.proto
protoc.exe --csharp_out="./" --proto_path="./" NFMsgBase.proto
protoc.exe --csharp_out="./" --proto_path="./" NFMsgPreGame.proto
protoc.exe --csharp_out="./" --proto_path="./" NFMsgShare.proto
protoc.exe --csharp_out="./" --proto_path="./" NFMsgExtra.proto

xcopy *.cs ..\..\NFClient\NFMessageDefine\ /s /e /Y
