var html_ServerStatus_temp;
html_ServerStatus_temp = "\
	<div class='col-lg-3 col-sm-6'>\
		<div class='card'>\
			<div class='content'>\
				<div class='row'>\
					<div class='col-xs-5'>\
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
					<p class='category'>Online Count</p>\
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
		}
		document.getElementById("serverChartList").innerHTML = "<div class='row'>" + html_ServerStatus + "</div>" + html_ServerLine;
		
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

			Chartist.Line('#' + ServerNameList[i] + "ID", data, options, responsive);
		}

	}
}

function getMasterInfo()
{
	$.getJSON("http://" + window.localStorage.getItem("formServerIP") + ":" + window.localStorage.getItem("formServerPort") + "/id=10200,jsoncallback=?", function (data) {
		var serverNameList = new Array();
		var serverStatusList = new Array();
		var serverUpdateList = new Array();
		var serverOnlineList = new Array();
		var serverMaxList = new Array();

		serverNowTime = data.nowTime;
		labelArr.push(serverNowTime);

		if(labelArr.length > 8)
		{
			labelArr.shift();
		}
		
		$.each(data.master, function (i, item) {
			serverNameList.push(item.servrName);
			serverStatusList.push(item.status);
			serverUpdateList.push(serverNowTime);
			
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
		
		showServerStatus(serverNameList, serverStatusList, serverUpdateList, serverOnlineList, serverMaxList);
	});
}


$(document).ready(function(){
	if(window.localStorage.getItem("formServerIP") && window.localStorage.getItem("formServerPort"))
	{
		setInterval(getMasterInfo,1000);
		$("#MasterInfoForm").hide();
		document.getElementById("MSIPPORT").innerHTML = "[" + window.localStorage.getItem("formServerIP") + ":" + window.localStorage.getItem("formServerPort") + "]";
		document.getElementById("MSIPPORT").innerHTML += " (click to edit)";
		document.getElementById("MSIPPORT").className = "text-success";
		$("#MSIPPORT").click(function(){
			$("#MasterInfoForm").show();
		});
		$("#formServerIP").val(window.localStorage.getItem("formServerIP"));
		$("#formServerPort").val(window.localStorage.getItem("formServerPort"));
	}
	$.notify({
		icon: 'ti-gift',
		message: "Welcome to <b>NoahGameFrame Web Monitor</b>."

	},{
		type: 'info',
		timer: 4000
	});
});

function reloadWeb(){location.reload();}

$("#serverButton").click(function(){  
	window.localStorage.setItem("formServerIP", $("#formServerIP").val());
	window.localStorage.setItem("formServerPort", $("#formServerPort").val());

	$.notify({
		icon: 'ti-gift',
		message: "Set IP[" + window.localStorage.getItem("formServerIP") + "] and Port[" + window.localStorage.getItem("formServerPort") + "] successfully."
	},{
		type: 'info',
		timer: 10
	});
	$("#MasterInfoForm").hide();
	setTimeout(function(){location.reload();},2000);
}); 
