@echo off
::系统环境变量
::HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment
::用户环境变量
::HKEY_CURRENT_USER\Environment


REM set regOgrepath=HKEY_CURRENT_USER\Environment

REM ::BIN目录
REM set regBINName=NFBIN_HOME
REM set regBINValue=D:\NoahGameFrame\trunk\_Out\Client\Bin
REM reg add "%regOgrepath%" /v "%regBINName%" /d "%regBINValue%" /f

REM ::SERVER_BIN目录
REM set regSERVERBINName=NFBIN_SERVER
REM set regSERVERBINValue=D:\NoahGameFrame\trunk\_Out\Server
REM reg add "%regOgrepath%" /v "%regSERVERBINName%" /d "%regSERVERBINValue%" /f

REM ::COMM_BIN目录
REM set regCOMMBINName=NFBIN_COMM
REM set regCOMMBINValue=D:\NoahGameFrame\trunk\_Out\Comm
REM reg add "%regOgrepath%" /v "%regCOMMBINName%" /d "%regCOMMBINValue%" /f


REM ::NF_HOME目录
REM set regNF_HOMEName=NF_HOME
REM set regNF_HOMEValue=D:\NoahGameFrame\trunk\\
REM reg add "%regOgrepath%" /v "%regNF_HOMEName%" /d "%regNF_HOMEValue%" /f

REM ::APP_LIB目录
REM set regAPP_LIBName=APP_DEPLIB
REM set regAPP_LIBVALUE=D:\NoahGameFrame\trunk\Dependencies\lib\\
REM reg add "%regOgrepath%" /v "%regAPP_LIBName%" /d "%regAPP_LIBVALUE%" /f

REM ::NF_NOSQL_NET目录
REM set regNF_HOMEName=NF_NOSQL_NET
REM set regNF_HOMEValue=D:\NoahGameFrame\trunk\Dependencies\redis-cplusplus-client\msvc\anet_win32\\
REM reg add "%regOgrepath%" /v "%regNF_HOMEName%" /d "%regNF_HOMEValue%" /f

REM ::BOOST
REM set regBOOSTName=BOOST_HOME
REM set regBOOSTValue=D:\NoahGameFrame\trunk\Dependencies\boost_1_53_0\\
REM reg add "%regOgrepath%" /v "%regBOOSTName%" /d "%regBOOSTValue%" /f

REM ::BOOSTLIB
REM set regBOOSTLIBName=BOOST_LIB
REM set regBOOSTLIBValue=D:\NoahGameFrame\trunk\Dependencies\boost_1_53_0\stage\lib\x64\vs11_0
REM reg add "%regOgrepath%" /v "%regBOOSTLIBName%" /d "%regBOOSTLIBValue%" /f

REM ::Dependencies
REM set regDEPName=NF_DEP
REM set regDEPNameValue=D:\NoahGameFrame\trunk\Dependencies\\
REM reg add "%regOgrepath%" /v "%regDEPName%" /d "%regDEPNameValue%" /f

REM ::GLOG目录
REM set regGLOGName=GLOG_HOME
REM set regGLOGValue=D:\NoahGameFrame\trunk\Dependencies\glog-0.3.3\src\windows\\
REM reg add "%regOgrepath%" /v "%regGLOGName%" /d "%regGLOGValue%" /f

REM ::Theron目录
REM set regTHERONName=THERON_HOME
REM set regTHERONValue=D:\NoahGameFrame\trunk\Dependencies\Theron-6.00.01\Include\\
REM reg add "%regOgrepath%" /v "%regTHERONName%" /d "%regTHERONValue%" /f

REM echo Set Environment end

rem ====================================================================================
echo Start to extract boost_1_53_0

cd Tool
7z x "..\Dependencies\boost_1_53_0.zip" -o"..\Dependencies\" -y

echo Extract boost_1_53_0 completed!
rem ====================================================================================

echo Start to extract nessasery lib

7z x "..\Dependencies\lib.zip" -o"..\Dependencies\" -y

echo Extract nessasery lib completed!

7z x "..\Dependencies\mono-3.6.0.39\mono-3.6.0.39.rar" -o"..\Dependencies\mono-3.6.0.39\" -y

echo Extract mono-3.6.0.39 completed!

echo Start to extract google-breakpad lib

7z x "..\Dependencies\google-breakpad.zip" -o"..\Dependencies\google-breakpad\" -y

echo Extract mono-3.6.0.39 completed!
rem ====================================================================================

pause