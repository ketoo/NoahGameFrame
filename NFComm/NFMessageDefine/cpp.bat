
protoc -I=./ --cpp_out=./ ./NFDefine.proto
protoc -I=./ --cpp_out=./ ./NFMsgBase.proto
protoc -I=./ --cpp_out=./ ./NFMsgPreGame.proto
protoc -I=./ --cpp_out=./ ./NFMsgShare.proto
protoc -I=./ --cpp_out=./ ./NFMsgExtra.proto
protoc -I=./ --cpp_out=./ ./NFMsgFriend.proto
protoc -I=./ --cpp_out=./ ./NFMsgTeam.proto


pause