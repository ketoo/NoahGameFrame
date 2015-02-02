<%@ Register TagPrefix="cc1" Namespace="GHTWebControls" Assembly="MainsoftWebApp" %>
<%@ Page Language="c#" AutoEventWireup="false" Codebehind="Repeater_AlternatingItemTemplate.aspx.cs" Inherits="GHTTests.System_Web_dll.System_Web_UI_WebControls.Repeater_AlternatingItemTemplate" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
	<HEAD>
		<title>Repeater_AlternatingItemTemplate</title>
		<meta content="Microsoft Visual Studio .NET 7.1" name="GENERATOR">
		<meta content="Visual Basic .NET 7.1" name="CODE_LANGUAGE">
		<meta content="JavaScript" name="vs_defaultClientScript">
		<meta content="http://schemas.microsoft.com/intellisense/ie5" name="vs_targetSchema">
		<script LANGUAGE="JavaScript">
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
			<cc1:GHTSubTest id="GHTSubTest1" runat="server" Width="553px" Height="88px">
				<asp:Repeater id="Repeater1" runat="server">
					<ItemTemplate>
						<%# Container.DataItem %>
					</ItemTemplate>
					<AlternatingItemTemplate>
					</AlternatingItemTemplate>
				</asp:Repeater>
			</cc1:GHTSubTest>
			<cc1:GHTSubTest id="GHTSubTest2" runat="server" Height="88px" Width="553px">
				<asp:Repeater id="Repeater2" runat="server">
					<ItemTemplate>
						<%# Container.DataItem %>
					</ItemTemplate>
					<AlternatingItemTemplate>
						plain text as alternative item template
					</AlternatingItemTemplate>
				</asp:Repeater>
			</cc1:GHTSubTest>
			<cc1:GHTSubTest id="GHTSubTest3" runat="server" Height="88px" Width="553px">
				<asp:Repeater id="Repeater3" runat="server">
					<ItemTemplate>
						<%# Container.DataItem %>
					</ItemTemplate>
					<AlternatingItemTemplate>
						<div><span><b><i>html as alternating item template</i></b></span></div>
					</AlternatingItemTemplate>
				</asp:Repeater>
			</cc1:GHTSubTest>
			<cc1:GHTSubTest id="GHTSubTest4" runat="server" Height="88px" Width="553px">
				<asp:Repeater id="Repeater4" runat="server">
					<ItemTemplate>
						<asp:RadioButton runat="server" Text="<%# Container.DataItem %>" ID="Checkbox2"/>
					</ItemTemplate>
					<AlternatingItemTemplate>
						<asp:CheckBox runat="server" Text="<%# Container.DataItem %>" ID="Checkbox1"/>
					</AlternatingItemTemplate>
				</asp:Repeater>
			</cc1:GHTSubTest>
			<cc1:GHTSubTest id="GHTSubTest5" runat="server" Height="88px" Width="553px">
				<asp:Repeater id="Repeater5" runat="server">
					<HeaderTemplate><table border="1"></HeaderTemplate>
					<ItemTemplate>
						<tr bgcolor="#777777"><td><%# Container.DataItem %></td></tr>
					</ItemTemplate>
					<AlternatingItemTemplate>
						<tr bgcolor="#eeeeee"><td><%# Container.DataItem %></td></tr>
					</AlternatingItemTemplate>
					<FooterTemplate></table></FooterTemplate>
				</asp:Repeater>
			</cc1:GHTSubTest>
		</form>
		<br>
		<br>
	</body>
</HTML>
