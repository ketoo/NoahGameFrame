echo on

rem "%VS140COMNTOOLS%..\IDE\Devenv" NFMessage.sln /Rebuild "Release|X64"

.\Precompile\precompile.exe -o:.\bin\Release\NFMessage.serializer.dll -t:NFMsg.Serializer .\bin\Release\NFMessage.dll


rem xcopy .\bin\Release\*.dll ..\..\..\.\Assets\Plugins\ /s /e /Y

pause