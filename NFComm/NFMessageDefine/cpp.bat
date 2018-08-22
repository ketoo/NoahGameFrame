
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFDefine.proto
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgBase.proto
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgExtra.proto
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgPreGame.proto
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgShare.proto

pause