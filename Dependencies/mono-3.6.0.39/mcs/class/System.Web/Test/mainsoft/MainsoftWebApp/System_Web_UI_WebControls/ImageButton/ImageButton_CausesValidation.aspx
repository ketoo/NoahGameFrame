<%@ Register TagPrefix="cc1" Namespace="GHTWebControls" Assembly="MainsoftWebApp" %>
<%@ Page Language="c#" AutoEventWireup="false" Codebehind="ImageButton_CausesValidation.aspx.cs" Inherits="GHTTests.System_Web_dll.System_Web_UI_WebControls.ImageButton_CausesValidation" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
	<HEAD>
		<title>ImageButton_CausesValidation</title>
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
	<body MS_POSITIONING="GridLayout">
		<form id="Form1" method="post" runat="server">
			<cc1:GHTSubTest id="GHTSubTest1" style="Z-INDEX: 101; LEFT: 16px; POSITION: absolute; TOP: 15px"
				runat="server" Width="40px" Height="40px">
				<asp:ImageButton id="ImageButton1" runat="server" CausesValidation="False"></asp:ImageButton>
			</cc1:GHTSubTest>
			<cc1:GHTSubTest id="GHTSubTest3" style="Z-INDEX: 105; LEFT: 16px; POSITION: absolute; TOP: 112px"
				runat="server" Height="40px" Width="40px">
				<asp:ImageButton id="ImageButton3" runat="server" CausesValidation="False"></asp:ImageButton>
			</cc1:GHTSubTest>
			<cc1:GHTSubTest id="GHTSubTest2" style="Z-INDEX: 102; LEFT: 16px; POSITION: absolute; TOP: 64px"
				runat="server" Height="40px" Width="40px">
				<asp:ImageButton id="ImageButton2" runat="server"></asp:ImageButton>
			</cc1:GHTSubTest>&nbsp;
			<asp:TextBox id="TextBox1" style="Z-INDEX: 104; LEFT: 88px; POSITION: absolute; TOP: 24px" runat="server"></asp:TextBox>
			<br>
			<br>
			<asp:RequiredFieldValidator id="RequiredFieldValidator1" style="Z-INDEX: 103; LEFT: 88px; POSITION: absolute; TOP: 72px"
				runat="server" ErrorMessage="RequiredFieldValidator" ControlToValidate="TextBox1"></asp:RequiredFieldValidator>
		</form>
	</body>
</HTML>
