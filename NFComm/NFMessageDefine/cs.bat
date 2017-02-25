echo on
protogen -i:NFDefine.proto -o:NFDefine.cs
protogen -i:NFMsgBase.proto -o:NFMsgBase.cs
protogen -i:NFMsgPreGame.proto -o:NFMsgPreGame.cs
protogen -i:NFMsgShare.proto -o:NFMsgShare.cs
protogen -i:NFSLGDefine.proto -o:NFSLGDefine.cs
protogen -i:NFFleetingDefine.proto -o:NFFleetingDefine.cs
protogen -i:NFMsgURl.proto -o:NFMsgURl.cs

xcopy *.cs ..\..\NFClient\NFMessageDefine\ /s /e /Y

pause