var html_ServerStatus_temp;
html_ServerStatus_temp = "\
	<div class='col-lg-2 col-sm-6'>\
		<div class='card'>\
			<div class='content'>\
				<div class='row'>\
					<div class='col-xs-3'>\
						<div class='icon-big {$Color} text-center'>\
							<i class='ti-light-bulb'></i>\
						</div>\
					</div>\
					<div class='col-xs-7'>\
						<div class='numbers'>\
							<p>{$ServerName}</p>\
							{$ServerStatus}\
						</div>\
					</div>\
				</div>\
				<div class='footer'>\
					<hr />\
					<div class='stats'>\
						<i class='ti-reload'></i>\
						{$UpdateTime}\
					</div>\
				</div>\
			</div>\
		</div>\
	</div>";
var html_ServerLine_temp;
html_ServerLine_temp = "\
	<div class='row'>\
		<div class='col-md-12'>\
			<div class='card'>\
				<div class='header'>\
					<h4 class='title'>{$serverName}</h4>\
					<p class='category'>Online Count [Now {$NowCount}]</p>\
				</div>\
				<div class='content'>\
					<div id='{$serverNameId}' class='ct-chart'></div>\
					<div class='footer'>\
						<hr>\
						<div class='stats'>\
							<i class='ti-reload'></i>\
							Updated 1 second ago\
						</div>\
					</div>\
				</div>\
			</div>\
		</div>\
	</div>";
var html_ServerInfoTable_temp;
html_ServerInfoTable_temp = "\
	<div class='row'>\
		<div class='col-md-12'>\
			<div class='card'>\
				<div class='header'>\
					<h4 class='title'>{$TableTitle}</h4>\
					<p class='category'>{$TableDescript}</p>\
				</div>\
				<div class='content table-responsive table-full-width'>\
					<table class='table table-striped'>\
						<thead>\
							<th>ID</th>\
							<th>InfoName</th>\
							<th>InfoValue</th>\
						</thead>\
						<tbody>\
						{$InfoTableTr}\
						</tbody>\
					</table>\
				</div>\
			</div>\
		</div>\
	</div>"
var html_ServerInfoTableTr = "\
							<tr id='InfoTableTr' class='table-tr-extra'>\
								<td>{$RowID}</td>\
								<td>{$RowKey}</td>\
								<td>{$RowValue}</td>\
							</tr>"
var html_Nav_li = "\
                <li name='left_bar' class='{$li_active}' onclick='{$li_Script}'>\
                    <a href='#'>\
                        <i class='{$li_class}'></i>\
                        <p>{$li_name}</p>\
                    </a>\
                </li>"

var intervalTrigger;
var serverNowTime;
var chartMaxMaster;
var chartMaxWorld;
var chartMaxGame;
var chartMaxProxy;
var chartMaxLogin;
chartMaxMaster = 10;
chartMaxWorld = 10;
chartMaxGame = 10;
chartMaxProxy = 10;
chartMaxLogin = 10;
var labelArr = new Array();
var masterOnline = new Array();
var worldOnline = new Array();
var gameOnline = new Array();
var proxyOnline = new Array();
var loginOnline = new Array();
var chartList = new Array();
var currentPageHtml = "Dashboard";
var nShowBarCount = 0;
function showNavBar(CurrentPage, ServerNameList, ServerIDList)
{
	if(nShowBarCount != 0)
	{
		if(nShowBarCount % 5 != 0)
		{
			nShowBarCount++;
			return;
		}
	}
	nShowBarCount++;
	if(nShowBarCount > 10)
	{
		nShowBarCount = 0;
	}
	var serverClass = "ti-view-list-alt";
	var dashboardClass = "ti-panel";
	var html_Nav_bar = html_Nav_li;
	html_Nav_bar = html_Nav_bar.replace("{$li_name}", "Dashboard");
	html_Nav_bar = html_Nav_bar.replace("{$li_name}", "Dashboard");
	if(CurrentPage == "Dashboard")
	{
		html_Nav_bar = html_Nav_bar.replace("{$li_active}", "active");
	}
	else
	{
		html_Nav_bar = html_Nav_bar.replace("{$li_active}", "");
	}
	html_Nav_bar = html_Nav_bar.replace("{$li_class}", dashboardClass);
	html_Nav_bar = html_Nav_bar.replace("{$li_Script}", "currentPageHtml=\"Dashboard\";\
			getMasterInfo();\
			clearInterval(intervalTrigger);\
			intervalTrigger = setInterval(getMasterInfo,1000);\
			clearActiveLeftBar();\
			this.className =\"active\";\
			");

	for(var i=0;i<ServerNameList.length;i++)
	{
		html_Nav_bar += html_Nav_li;
		html_Nav_bar = html_Nav_bar.replace("{$li_name}", ServerNameList[i]);
		html_Nav_bar = html_Nav_bar.replace("{$li_name}", ServerNameList[i]);
		if(CurrentPage == ServerNameList[i])
		{
			html_Nav_bar = html_Nav_bar.replace("{$li_active}", "active");
		}
		else
		{
			html_Nav_bar = html_Nav_bar.replace("{$li_active}", "");
		}
		html_Nav_bar = html_Nav_bar.replace("{$li_class}", serverClass);
		html_Nav_bar = html_Nav_bar.replace("{$li_Script}", "\
			currentPageHtml=\""+ServerNameList[i]+"\";\
			getMasterInfo();\
			clearInterval(intervalTrigger);\
			intervalTrigger = setInterval(getMasterInfo,1000);\
			clearActiveLeftBar();\
			this.className =\"active\";\
			");
	}
	document.getElementById("Nav_ul").innerHTML = html_Nav_bar;
}

function showServerStatus(ServerNameList, ServerStatusList, UpdateTimeList, ServerOnlineList, ServerMaxList)
{
	if(typeof(ServerNameList) != "undefined")
	{
		var html_ServerStatus;
		html_ServerStatus = "";
		var html_ServerLine;
		html_ServerLine = "";
		for(var i=0;i<ServerNameList.length;i++){
			html_ServerStatus += html_ServerStatus_temp.replace("{$ServerName}", ServerNameList[i]);
			html_ServerStatus = html_ServerStatus.replace("{$UpdateTime}", UpdateTimeList[i]);
			if(ServerStatusList[i] == 1)
			{
				html_ServerStatus = html_ServerStatus.replace("{$Color}", "icon-success");
				html_ServerStatus = html_ServerStatus.replace("{$ServerStatus}", "Online");
			}
			else
			{
				html_ServerStatus = html_ServerStatus.replace("{$Color}", "icon-warning");
				html_ServerStatus = html_ServerStatus.replace("{$ServerStatus}", "Offline");
			}
			
			html_ServerLine += html_ServerLine_temp.replace("{$serverName}", ServerNameList[i]);
			html_ServerLine = html_ServerLine.replace("{$serverNameId}", ServerNameList[i] + "ID");
            html_ServerLine = html_ServerLine.replace("{$NowCount}", ServerOnlineList[i][0][ServerOnlineList[i][0].length-1]);
		}
		document.getElementById("serverContent").innerHTML = "<div class='row'>" + html_ServerStatus + "</div>" + html_ServerLine;
		
		for(var i=0;i<ServerNameList.length;i++)
		{
			var data = {
				labels: labelArr,
				series: 
				ServerOnlineList[i]
			};
			
			var options = {
			  lineSmooth: false,
			  low: 0,
			  high: ServerMaxList[i],
			  showArea: false,
			  height: "245px",
			  axisX: {
				showGrid: false,
			  },
			  axisY: {
			  	onlyInteger: true,
			  },
			  lineSmooth: Chartist.Interpolation.simple({
				divisor: 5
			  }),
			  showLine: true,
			  showPoint: false,
			};
			
			var responsive = [
			  ['screen and (max-width: 640px)', {
				axisX: {
				  labelInterpolationFnc: function (value) {
					return value[0];
				  }
				}
			  }]
			];		    	

			var chart = Chartist.Line('#' + ServerNameList[i] + "ID", data, options, responsive);
			chartList.push(chart);			
			while(chartList.length > 20)
			{
				var chartToDel = chartList.shift();
				chartToDel.detach();
				chartToDel = null;
			}
		}
	}
}

function showServerInfoTable(CurrentServerID, ServerNameList, ServerStatusList, ServerUpdateList, ServerIPList, ServerPortList, ServerIDList, serverOnlineList, ServerInfoExt)
{
	if(typeof(ServerNameList) != "undefined")
	{
		var html_ServerInfoTable;
		html_ServerInfoTable = "";
		var nCol = 0;

		for(var i=0;i<ServerNameList.length;i++){
			if(ServerIDList[i] == CurrentServerID)
			{
				html_ServerInfoTable += html_ServerInfoTable_temp.replace("{$TableTitle}", ServerNameList[i] + " Basic Information");
				html_ServerInfoTable = html_ServerInfoTable.replace("{$TableDescript}", "Main Information");
				var html_tbody = "";
				var html_ServerInfoTable_Tr = html_ServerInfoTableTr;
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowID}", "1");
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowKey}", "serverId");
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowValue}", ServerIDList[i]);
				html_tbody += html_ServerInfoTable_Tr;
				html_ServerInfoTable_Tr = html_ServerInfoTableTr;
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowID}", "2");
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowKey}", "ServerName");
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowValue}", ServerNameList[i]);
				html_tbody += html_ServerInfoTable_Tr;
				html_ServerInfoTable_Tr = html_ServerInfoTableTr;
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowID}", "3");
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowKey}", "ServerIP");
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowValue}", ServerIPList[i]);
				html_tbody += html_ServerInfoTable_Tr;
				html_ServerInfoTable_Tr = html_ServerInfoTableTr;
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowID}", "4");
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowKey}", "ServerPort");
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowValue}", ServerPortList[i]);
				html_tbody += html_ServerInfoTable_Tr;
				html_ServerInfoTable_Tr = html_ServerInfoTableTr;
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowID}", "5");
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowKey}", "ServerOnlineCount");
				html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowValue}", serverOnlineList[i][0][serverOnlineList[i][0].length - 1]);
				html_tbody += html_ServerInfoTable_Tr;
				html_ServerInfoTable_Tr = html_ServerInfoTableTr;
				if(ServerStatusList[i] == 1)
				{
					html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowID}", "6");
					html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowKey}", "ServerStatus");
					html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowValue}", "Online");
				}
				else
				{
					html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowID}", "6");
					html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowKey}", "ServerStatus");
					html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowValue}", "Offline");
				}
				html_tbody += html_ServerInfoTable_Tr;

				if(ServerStatusList[i] == 1)
				{
					html_ServerInfoTable = html_ServerInfoTable.replace("{$Color}", "icon-success");
					html_ServerInfoTable = html_ServerInfoTable.replace("{$ServerStatus}", "Online");
				}
				else
				{
					html_ServerInfoTable = html_ServerInfoTable.replace("{$Color}", "icon-warning");
					html_ServerInfoTable = html_ServerInfoTable.replace("{$ServerStatus}", "Offline");
				}

				var dataList = ServerInfoExt[i];
				$.each(dataList, function (i, item) {
					html_ServerInfoTable_Tr = html_ServerInfoTableTr;
					html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowID}", i+7);
					$.each(item, function (key, value){
						html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowKey}", key);
						html_ServerInfoTable_Tr = html_ServerInfoTable_Tr.replace("{$RowValue}", value);
					});
					html_tbody += html_ServerInfoTable_Tr;
				});
				html_ServerInfoTable = html_ServerInfoTable.replace("{$InfoTableTr}", html_tbody);

			}
		}
		
		document.getElementById("serverContent").innerHTML = html_ServerInfoTable;

	}
}

function getMasterInfo()
{
	$.ajax({ 
		type: "post", 
		url: "json/dashboard", 
		dataType: "json", 
		success: function (data) {
			dataFunctionSwitch(data);
		}, 
		error: function (XMLHttpRequest, textStatus, errorThrown) { 
			$.notify({
				icon: 'ti-info',
				message: "Master Web Server Error!"
			},{
				type: 'danger ',
				delay: 2000,
			});
			clearInterval(intervalTrigger);
			return;
		} 
	});
}

function dataFunctionSwitch(data)
{
	if(typeof(data.code) == "undefined")
	{
		$.notify({
			icon: 'ti-info',
			message: "Master Web Server Error!"

		},{
			type: 'danger',
			delay: 2000,
		});
		clearInterval(intervalTrigger);
		return;
	}
	else
	{
		if(data.code != "0")
		{
			$.notify({
				icon: 'ti-info',
				message: "Master Web Server Error!["+ data.errMsg +"]"

			},{
				type: 'danger ',
				delay: 2000,
			});
			if(data.code != "0")
			{
				clearInterval(intervalTrigger);
				return;
			}
		}
	}

	var serverNameList = new Array();
	var serverStatusList = new Array();
	var serverUpdateList = new Array();
	var serverOnlineList = new Array();
	var serverMaxList = new Array();
	var serverIPList = new Array();
	var serverPortList = new Array();
	var serverIDList = new Array();
	var serverInfoExt = new Array();

	serverNowTime = data.nowTime;
	serverNowTime = new Date(data.nowTime * 1000);
	serverNowTime = pad(serverNowTime.getHours(),2) +":"+ pad(serverNowTime.getMinutes(),2) +":"+ pad(serverNowTime.getSeconds(),2);
	labelArr.push(serverNowTime);

	if(labelArr.length > 8)
	{
		labelArr.shift();
	}
	
	$.each(data.master, function (i, item) {
		serverNameList.push(item.servrName);
		serverStatusList.push(item.status);
		serverUpdateList.push(serverNowTime);
		serverIPList.push(item.ip);
		serverPortList.push(item.port);
		serverIDList.push(item.serverId);
		serverInfoExt.push(item.info_ext);

		if(typeof(masterOnline[i]) == "undefined")
		{
			masterOnline[i] = new Array();
		}
		masterOnline[i].push(item.onlineCount);
		if(masterOnline[i].length > 8)
		{
			masterOnline[i].shift();
		}
		if(chartMaxMaster < item.onlineCount + 10)
		{
			chartMaxMaster = item.onlineCount + 10;
		}
		serverOnlineList.push(masterOnline);
		serverMaxList.push(chartMaxMaster);
	});
	
	$.each(data.worlds, function (i, item) {
		serverNameList.push(item.servrName);
		serverStatusList.push(item.status);
		serverUpdateList.push(serverNowTime);
		serverIPList.push(item.ip);
		serverPortList.push(item.port);
		serverIDList.push(item.serverId);
		serverInfoExt.push(item.info_ext);

		if(typeof(worldOnline[i]) == "undefined")
		{
			worldOnline[i] = new Array();
		}
		worldOnline[i].push(item.onlineCount);
		if(worldOnline[i].length > 8)
		{
			worldOnline[i].shift();
		}
		if(chartMaxWorld < item.onlineCount + 10)
		{
			chartMaxWorld = item.onlineCount + 10;
		}
		serverOnlineList.push(worldOnline);
		serverMaxList.push(chartMaxWorld);
	});

	$.each(data.games, function (i, item) {
		serverNameList.push(item.servrName);
		serverStatusList.push(item.status);
		serverUpdateList.push(serverNowTime);
		serverIPList.push(item.ip);
		serverPortList.push(item.port);
		serverIDList.push(item.serverId);
		serverInfoExt.push(item.info_ext);

		if(typeof(gameOnline[i]) == "undefined")
		{
			gameOnline[i] = new Array();
		}
		gameOnline[i].push(item.onlineCount);
		if(gameOnline[i].length > 8)
		{
			gameOnline[i].shift();
		}
		if(chartMaxGame < item.onlineCount + 10)
		{
			chartMaxGame = item.onlineCount + 10;
		}
		serverOnlineList.push(gameOnline);
		serverMaxList.push(chartMaxGame);
	});

	$.each(data.proxys, function (i, item) {
		serverNameList.push(item.servrName);
		serverStatusList.push(item.status);
		serverUpdateList.push(serverNowTime);
		serverIPList.push(item.ip);
		serverPortList.push(item.port);
		serverIDList.push(item.serverId);
		serverInfoExt.push(item.info_ext);

		if(typeof(proxyOnline[i]) == "undefined")
		{
			proxyOnline[i] = new Array();
		}
		proxyOnline[i].push(item.onlineCount);
		if(proxyOnline[i].length > 8)
		{
			proxyOnline[i].shift();
		}
		if(chartMaxProxy < item.onlineCount + 10)
		{
			chartMaxProxy = item.onlineCount + 10;
		}
		serverOnlineList.push(proxyOnline);
		serverMaxList.push(chartMaxProxy);
	});

	$.each(data.logins, function (i, item) {
		serverNameList.push(item.servrName);
		serverStatusList.push(item.status);
		serverUpdateList.push(serverNowTime);
		serverIPList.push(item.ip);
		serverPortList.push(item.port);
		serverIDList.push(item.serverId);
		serverInfoExt.push(item.info_ext);

		if(typeof(loginOnline[i]) == "undefined")
		{
			loginOnline[i] = new Array();
		}
		loginOnline[i].push(item.onlineCount);
		if(loginOnline[i].length > 8)
		{
			loginOnline[i].shift();
		}
		if(chartMaxLogin < item.onlineCount + 10)
		{
			chartMaxLogin = item.onlineCount + 10;
		}
		serverOnlineList.push(loginOnline);
		serverMaxList.push(chartMaxLogin);
	});

	showNavBar(currentPageHtml, serverNameList, serverIDList);

	if(currentPageHtml == "Dashboard")
	{
		showServerStatus(serverNameList, serverStatusList, serverUpdateList, serverOnlineList, serverMaxList);
	}
	else
	{
		for(var i=0;i<serverNameList.length;i++)
		{

			if(serverNameList[i] == currentPageHtml)
			{

				showServerInfoTable(serverIDList[i], serverNameList, serverStatusList, serverUpdateList, serverIPList, serverPortList, serverIDList, serverOnlineList, serverInfoExt);
			}
		}		
	}

	document.getElementById("PageTitle").innerHTML = currentPageHtml;

	serverNameList.length = 0;
	serverStatusList.length = 0;
	serverUpdateList.length = 0;
	serverOnlineList.length = 0;
	serverMaxList.length = 0;
	serverIPList.length = 0;
	serverPortList.length = 0;
	serverIDList.length = 0;
	serverInfoExt.length = 0;
}


$(document).ready(function(){
	intervalTrigger = setInterval(getMasterInfo,1000);

	$.notify({
		icon: 'ti-gift',
		message: "Welcome to <b>NoahFrame Web Monitor</b>."

	},{
		type: 'info',
		timer: 4000
	});
});

function clearActiveLeftBar()
{
	var BarList = document.getElementsByName("left_bar");
	for(var i =0; i< BarList.length; i++)
	{
		BarList[i].className ="";
	}
	
}

function pad(num, n) {
    var len = num.toString().length;
    while(len < n) {
        num = '0' + num;
        len++;
    }
    return num;
}