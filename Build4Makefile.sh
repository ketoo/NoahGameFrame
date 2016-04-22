# delete old makefile(s)
find ../NFComm -name 'makefile' -type f -print -exec rm -rf {} \;
find ../NFServer -name 'makefile' -type f -print -exec rm -rf {} \;

# start to generate makefile(s)
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
./cbp2make.linux-x86_64 -in ../NFServer/NFProxyServerPlugin/NFProxyPlugin.cbp -out ../NFServer/NFProxyServerPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFWorldLogicPlugin/NFWorldLogicPlugin.cbp -out ../NFServer/NFWorldLogicPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFWorldNet_ClientPlugin/NFWorldNet_ClientPlugin.cbp -out ../NFServer/NFWorldNet_ClientPlugin/makefile
./cbp2make.linux-x86_64 -in ../NFServer/NFWorldNet_ServerPlugin/NFWorldNet_ServerPlugin.cbp -out ../NFServer/NFWorldNet_ServerPlugin/makefile


# add SolutionDir in line 20
# SolutionDir = ../../
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFCluster/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFConfigPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFCore/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFCrashPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFDataBasePlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFDataNoSqlPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFKernelPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFLogPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFLuaScriptPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFMessageDefine/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFNet/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFPluginLoader/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFTagPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFComm/NFUrlPlugin/makefile

sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFDataBaseNet_ClientPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFGameServerNet_ClientPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFGameServerNet_ServerPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFGameServerPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFLoginLogicPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFLoginNet_ClientPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFLoginNet_ServerPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFMasterNet_ServerPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFMasterServerPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFProxyServerNet_ClientPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFProxyServerNet_ServerPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFProxyServerPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFWorldLogicPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFWorldNet_ClientPlugin/makefile
sed -i '20i\SolutionDir = ../../\n' ../NFServer/NFWorldNet_ServerPlugin/makefile



