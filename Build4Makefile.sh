
# start to generate makefile
cd ./Tool

./cbp2make.linux-x86_64 -in ../NFComm/NFCluster/NFMysqlClusterPlugin.cbp -out ../NFComm/NFCluster/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFConfigPlugin/NFConfigPlugin.cbp -out ../NFComm/NFConfigPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFCore/NFCore.cbp -out ../NFComm/NFCore/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFCrashPlugin/NFCrashPlugin.cbp -out ../NFComm/NFCrashPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFDataBasePlugin/NFDataBasePlugin.cbp -out ../NFComm/NFDataBasePlugin/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFDataNoSqlPlugin/NFDataNoSqlPlugin.cbp -out ../NFComm/NFDataNoSqlPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFKernelPlugin/NFKernelPlugin.cbp -out ../NFComm/NFKernelPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFLogPlugin/NFLogPlugin.cbp -out ../NFComm/NFLogPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFLuaScriptPlugin/NFLuaScriptPlugin.cbp -out ../NFComm/NFLuaScriptPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFMessageDefine/NFMessageDefine.cbp -out ../NFComm/NFMessageDefine/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFNet/NFNet.cbp -out ../NFComm/NFNet/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFPluginLoader/NFPluginLoader.cbp -out ../NFComm/NFPluginLoader/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFTagPlugin/NFTagPlugin.cbp -out ../NFComm/NFTagPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFComm/NFUrlPlugin/NFUrlPlugin.cbp -out ../NFComm/NFUrlPlugin/makefile

./cbp2make.linux-x86_64 -in ../NFServer/NFDataBaseNet_ClientPlugin/NFDataBaseNet_ClientPlugin.cbp -out ../NFServer/NFDataBaseNet_ClientPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFGameServerNet_ClientPlugin/NFGameServerNet_ClientPlugin.cbp -out ../NFServer/NFGameServerNet_ClientPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFGameServerNet_ServerPlugin/NFGameServerNet_ServerPlugin.cbp -out ../NFServer/NFGameServerNet_ServerPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFGameServerPlugin/NFGameServerPlugin.cbp -out ../NFServer/NFGameServerPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFLoginLogicPlugin/NFLoginLogicPlugin.cbp -out ../NFServer/NFLoginLogicPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFLoginNet_ClientPlugin/NFLoginNet_ClientPlugin.cbp -out ../NFServer/NFLoginNet_ClientPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFLoginNet_ServerPlugin/NFLoginNet_ServerPlugin.cbp -out ../NFServer/NFLoginNet_ServerPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFMasterNet_ServerPlugin/NFMasterNet_ServerPlugin.cbp -out ../NFServer/NFMasterNet_ServerPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFMasterServerPlugin/NFMasterServerPlugin.cbp -out ../NFServer/NFMasterServerPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFProxyServerNet_ClientPlugin/NFProxyServerNet_ClientPlugin.cbp -out ../NFServer/NFProxyServerNet_ClientPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFProxyServerNet_ServerPlugin/NFProxyServerNet_ServerPlugin.cbp -out ../NFServer/NFProxyServerNet_ServerPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFProxyPlugin/NFProxyPlugin.cbp -out ../NFServer/NFProxyPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFWorldLogicPlugin/NFWorldLogicPlugin.cbp -out ../NFServer/NFWorldLogicPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFWorldNet_ClientPlugin/NFWorldNet_ClientPlugin.cbp -out ../NFServer/NFWorldNet_ClientPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFWorldNet_ServerPlugin/NFWorldNet_ServerPlugin.cbp -out ../NFServer/NFWorldNet_ServerPlugin/makefile

# add SolutionDir
# SolutionDir = ../../
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFCluster/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFConfigPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFCore/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFCrashPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFDataBasePlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFDataNoSqlPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFKernelPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFLogPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFLuaScriptPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFMessageDefine/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFNet/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFPluginLoader/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFTagPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFUrlPlugin/makefile

sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFDataBaseNet_ClientPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFGameServerNet_ClientPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFGameServerNet_ServerPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFGameServerPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFLoginLogicPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFLoginNet_ClientPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFLoginNet_ServerPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFMasterNet_ServerPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFMasterServerPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFProxyServerNet_ClientPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFProxyServerNet_ServerPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFProxyPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFWorldLogicPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFWorldNet_ClientPlugin/makefile
sed '/LDFLAGS = /a\SolutionDir = ../../\n' ../NFComm/NFWorldNet_ServerPlugin/makefile


cd ../