./install.sh


cd ./Tool

./cbp2make.linux-x86_64 - -in ../NFComm/NFMysqlClusterPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFConfigPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFCore.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFCrashPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFDataBasePlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFDataNoSqlPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFKernelPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFLogPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFLuaScriptPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFMessageDefine.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFNet.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFPluginLoader.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFTagPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFComm/NFUrlPlugin.cbp -out makefile

./cbp2make.linux-x86_64 - -in ../NFServer/NFDataBaseNet_ClientPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFGameServerNet_ClientPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFGameServerNet_ServerPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFGameServerPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFLoginLogicPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFLoginNet_ClientPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFLoginNet_ServerPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFMasterNet_ServerPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFMasterServerPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFProxyServerNet_ClientPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFProxyServerNet_ServerPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFProxyPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFWorldLogicPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFWorldNet_ClientPlugin.cbp -out makefile
./cbp2make.linux-x86_64 - -in ../NFServer/NFWorldNet_ServerPlugin.cbp -out makefile

cd ..
--SolutionDir = ../../