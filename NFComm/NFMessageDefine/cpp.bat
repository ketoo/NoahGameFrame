
protoc.exe -I=./ --cpp_out=./ ./NFDefine.proto 
protoc.exe -I=./ --cpp_out=./ ./NFMsgBase.proto 
protoc.exe -I=./ --cpp_out=./ ./NFMsgPreGame.proto 
protoc.exe -I=./ --cpp_out=./ ./NFMsgShare.proto  
protoc.exe -I=./ --cpp_out=./ ./NFSLGDefine.proto 
protoc.exe -I=./ --cpp_out=./ ./NFFleetingDefine.proto  
protoc.exe -I=./ --cpp_out=./ ./NFMsgURl.proto 

pause