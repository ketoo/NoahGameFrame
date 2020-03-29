chmod 777 protoc

./protoc -I=./ --cpp_out=./ ./NFDefine.proto
./protoc -I=./ --cpp_out=./ ./NFMsgBase.proto
./protoc -I=./ --cpp_out=./ ./NFMsgPreGame.proto
./protoc -I=./ --cpp_out=./ ./NFMsgShare.proto
