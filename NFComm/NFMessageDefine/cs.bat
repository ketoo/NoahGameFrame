echo on
protogen -i:NFDefine.proto -o:NFDefine.cs
protogen -i:NFMsgBase.proto -o:NFMsgBase.cs
protogen -i:NFMsgBaseEx.proto -o:NFMsgBaseEx.cs
protogen -i:NFMsgPreGame.proto -o:NFMsgPreGame.cs
protogen -i:NFMsgShare.proto -o:NFMsgShare.cs
protogen -i:NFSLGDefine.proto -o:NFSLGDefine.cs
protogen -i:NFFleetingDefine.proto -o:NFFleetingDefine.cs
protogen -i:NFRecordDefine.proto -o:NFRecordDefine.cs

pause