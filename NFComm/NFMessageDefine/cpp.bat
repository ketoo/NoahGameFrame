
protoc.exe -I=./ --cpp_out=./ ./NFDefine.proto
protoc.exe -I=./ --cpp_out=./ ./NFMsgBase.proto
protoc.exe -I=./ --cpp_out=./ ./NFMsgPreGame.proto
protoc.exe -I=./ --cpp_out=./ ./NFMsgShare.proto
protoc.exe -I=./ --cpp_out=./ ./NFMsgExtra.proto
protoc.exe -I=./ --cpp_out=./ ./NFMsgFriend.proto
protoc.exe -I=./ --cpp_out=./ ./NFMsgTeam.proto


pause