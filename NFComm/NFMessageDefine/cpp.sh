
protoc -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFDefine.proto
protoc -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgBase.proto
protoc -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgPreGame.proto
protoc -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgShare.proto
protoc -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./NFMsgExtra.proto
