kill -9 $(pidof NFMasterServer_d)
kill -9 $(pidof NFWorldServer_d)
kill -9 $(pidof NFLoginServer_d)
kill -9 $(pidof NFGameServer_d)
kill -9 $(pidof NFProxyServer_d)

ps -A|grep NF
