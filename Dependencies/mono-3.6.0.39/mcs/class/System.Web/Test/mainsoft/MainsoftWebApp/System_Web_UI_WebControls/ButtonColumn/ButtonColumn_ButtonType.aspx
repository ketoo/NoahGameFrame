<%@ Page Language="c#" AutoEventWireup="false" Codebehind="ButtonColumn_ButtonType.aspx.cs" Inherits="GHTTests.System_Web_dll.System_Web_UI_WebControls.ButtonColumn_ButtonType" %>
<%@ Register TagPrefix="cc1" Namespace="GHTWebControls" Assembly="MainsoftWebApp" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
	<HEAD>
		<title>ButtonColumn_ButtonType</title>
		<meta content="Microsoft Visual Studio .NET 7.1" name="GENERATOR">
		<meta content="Visual Basic .NET 7.1" name="CODE_LANGUAGE">
		<meta content="JavaScript" name="vs_defaultClientScript">
		<meta content="http://schemas.microsoft.com/intellisense/ie5" name="vs_targetSchema">
		<script language="JavaScript">
        function ScriptTest()
        {
            var theform;
		    if (window.navigator.appName.toLowerCase().indexOf("netscape") > -1) {
			    theform = document.forms["Form1"];
		    }
		    else {
			    theform = document.Form1;
		    }
        }
		</script>
	</HEAD>
	<body>
		<form id="Form1" method="post" runat="server">
			<cc1:ghtsubtest id="GHTSubTest1" runat="server" Height="120px" Width="553px">
				<asp:DataGrid id="DataGrid1" runat="server" AutoGenerateColumns="False"></asp:DataGrid>
			</cc1:ghtsubtest><cc1:ghtsubtest id="Ghtsubtest2" runat="server" Height="120px" Width="553px">
				<asp:DataGrid id="DataGrid2" runat="server" AutoGenerateColumns="False"></asp:DataGrid>
			</cc1:ghtsubtest><cc1:ghtsubtest id="Ghtsubtest3" runat="server" Height="120px" Width="553px">
				<asp:DataGrid id="DataGrid3" runat="server" AutoGenerateColumns="False"></asp:DataGrid>
			</cc1:ghtsubtest>
			<cc1:ghtsubtest id="Ghtsubtest4" runat="server" Height="120px" Width="553px">
				<asp:DataGrid id="DataGrid4" runat="server" AutoGenerateColumns="False">
					<Columns>
						<asp:ButtonColumn DataTextField="ID"></asp:ButtonColumn>
						<asp:ButtonColumn DataTextField="Name"></asp:ButtonColumn>
						<asp:ButtonColumn DataTextField="Company"></asp:ButtonColumn>
					</Columns>
				</asp:DataGrid>
			</cc1:ghtsubtest>
			<cc1:ghtsubtest id="Ghtsubtest5" runat="server" Height="120px" Width="553px">
				<asp:DataGrid id="DataGrid5" runat="server" AutoGenerateColumns="False">
					<Columns>
						<asp:ButtonColumn DataTextField="ID" ButtonType="LinkButton"></asp:ButtonColumn>
						<asp:ButtonColumn DataTextField="Name" ButtonType="LinkButton"></asp:ButtonColumn>
						<asp:ButtonColumn DataTextField="Company" ButtonType="LinkButton"></asp:ButtonColumn>
					</Columns>
				</asp:DataGrid>
			</cc1:ghtsubtest>
			<cc1:ghtsubtest id="Ghtsubtest6" runat="server" Height="120px" Width="553px">
				<asp:DataGrid id="DataGrid6" runat="server" AutoGenerateColumns="False">
					<Columns>
						<asp:ButtonColumn DataTextField="ID" ButtonType="PushButton"></asp:ButtonColumn>
						<asp:ButtonColumn DataTextField="Name" ButtonType="PushButton"></asp:ButtonColumn>
						<asp:ButtonColumn DataTextField="Company" ButtonType="PushButton"></asp:ButtonColumn>
					</Columns>
				</asp:DataGrid>
			</cc1:ghtsubtest></form>
	</body>
</HTML>
