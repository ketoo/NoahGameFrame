
del NFDefine.pb*
del NFMsgBase.pb*
del NFMsgPreGame.pb*
del NFMsgShare.pb*
protoc.exe -I=. --cpp_out=. ./NFDefine.proto
protoc.exe -I=. --cpp_out=. ./NFMsgBase.proto
protoc.exe -I=. --cpp_out=. ./NFMsgPreGame.proto
protoc.exe -I=. --cpp_out=. ./NFMsgShare.proto



pause