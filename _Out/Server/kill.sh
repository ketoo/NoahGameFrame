kill -9 $(pidof MasterSvr)
kill -9 $(pidof LoginSvr)
kill -9 $(pidof ProxySvr)
kill -9 $(pidof GameSvr)
kill -9 $(pidof WorldSvr)

ps -A|grep Svr
