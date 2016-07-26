kill -9 $(pidof NFMasterServer_r)
kill -9 $(pidof NFWorldServer_r)
kill -9 $(pidof NFLoginServer_r)
kill -9 $(pidof NFGameServer_r)
kill -9 $(pidof NFProxyServer_r)

ps -A|grep NF
