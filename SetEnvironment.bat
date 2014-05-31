@echo off
::系统环境变量
::HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment
::用户环境变量
::HKEY_CURRENT_USER\Environment


set regOgrepath=HKEY_CURRENT_USER\Environment

::BIN目录
set regBINName=NFBIN_HOME
set regBINValue=D:\NoahGameFrame\trunk\_Out\Client\Bin
reg add "%regOgrepath%" /v "%regBINName%" /d "%regBINValue%" /f

::SERVER_BIN目录
set regSERVERBINName=NFBIN_SERVER
set regSERVERBINValue=D:\NoahGameFrame\trunk\_Out\Server
reg add "%regOgrepath%" /v "%regSERVERBINName%" /d "%regSERVERBINValue%" /f

::COMM_BIN目录
set regCOMMBINName=NFBIN_COMM
set regCOMMBINValue=D:\NoahGameFrame\trunk\_Out\Comm
reg add "%regOgrepath%" /v "%regCOMMBINName%" /d "%regCOMMBINValue%" /f


::NF_HOME目录
set regNF_HOMEName=NF_HOME
set regNF_HOMEValue=D:\NoahGameFrame\trunk\\
reg add "%regOgrepath%" /v "%regNF_HOMEName%" /d "%regNF_HOMEValue%" /f

::APP_LIB目录
set regAPP_LIBName=APP_DEPLIB
set regAPP_LIBVALUE=D:\NoahGameFrame\trunk\Dependencies\lib\\
reg add "%regOgrepath%" /v "%regAPP_LIBName%" /d "%regAPP_LIBVALUE%" /f

::NF_NOSQL_NET目录
set regNF_HOMEName=NF_NOSQL_NET
set regNF_HOMEValue=D:\NoahGameFrame\trunk\Dependencies\redis-cplusplus-client\msvc\anet_win32\\
reg add "%regOgrepath%" /v "%regNF_HOMEName%" /d "%regNF_HOMEValue%" /f

::BOOST
set regBOOSTName=BOOST_HOME
set regBOOSTValue=D:\NoahGameFrame\trunk\Dependencies\boost_1_53_0\\
reg add "%regOgrepath%" /v "%regBOOSTName%" /d "%regBOOSTValue%" /f

::BOOSTLIB
set regBOOSTLIBName=BOOST_LIB
set regBOOSTLIBValue=D:\NoahGameFrame\trunk\Dependencies\boost_1_53_0\stage\lib\x64\vs11_0
reg add "%regOgrepath%" /v "%regBOOSTLIBName%" /d "%regBOOSTLIBValue%" /f

::Dependencies
set regDEPName=NF_DEP
set regDEPNameValue=D:\NoahGameFrame\trunk\Dependencies\\
reg add "%regOgrepath%" /v "%regDEPName%" /d "%regDEPNameValue%" /f

::GLOG目录
set regGLOGName=GLOG_HOME
set regGLOGValue=D:\NoahGameFrame\trunk\Dependencies\glog-0.3.3\src\windows\\
reg add "%regOgrepath%" /v "%regGLOGName%" /d "%regGLOGValue%" /f

::Theron目录
set regTHERONName=THERON_HOME
set regTHERONValue=D:\NoahGameFrame\trunk\Dependencies\Theron-6.00.01\Include\\
reg add "%regOgrepath%" /v "%regTHERONName%" /d "%regTHERONValue%" /f

echo Set Environment end

rem ====================================================================================
echo Start to extract boost_1_53_0

cd Tool
7z x "..\Dependencies\boost_1_53_0.zip" -o"..\Dependencies\" -y

echo Extract boost_1_53_0 completed!
rem ====================================================================================

echo Start to extract nessasery lib

7z x "..\Dependencies\lib.zip" -o"..\Dependencies\" -y

echo Extract nessasery lib completed!
rem ====================================================================================

pause