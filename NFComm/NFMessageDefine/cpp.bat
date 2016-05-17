
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFDefine.proto 
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgBase.proto 
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgPreGame.proto 
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgShare.proto  
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFSLGDefine.proto 
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFFleetingDefine.proto  
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgURl.proto 
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgMysql.proto 

pause