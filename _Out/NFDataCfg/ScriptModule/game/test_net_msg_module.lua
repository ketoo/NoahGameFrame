test_net_msg_module = {}

function test_net_msg_module:reload()
end

function test_net_msg_module:awake()
	test_net_msg_module:reload()
end

function test_net_msg_module:init()
end



function test_net_msg_module:after_init()
	script_module:log_info("test_net_msg_module after_init!----");
	script_module:add_msg_cb_as_client(NF_SERVER_TYPES.NF_ST_WORLD, 70, test_net_msg_module, test_net_msg_module.on_world_msg_process);
	script_module:add_msg_cb_as_server(100, test_net_msg_module, test_net_msg_module.on_proxy_msg_process);
end

function test_net_msg_module:ready_execute()
end

function test_net_msg_module:before_shut()
end

function test_net_msg_module:shut()
end

function test_net_msg_module:on_world_msg_process(fd, msgId, msgData)
	script_module:log_info("on_world_msg_process fd:" .. fd .. ", MsgId:" .. msgId);

end

function test_net_msg_module:on_proxy_msg_process(fd, msgId, msgData)
	script_module:log_info("on_proxy_msg_process fd:" .. fd .. ", MsgId:" .. msgId);

end

