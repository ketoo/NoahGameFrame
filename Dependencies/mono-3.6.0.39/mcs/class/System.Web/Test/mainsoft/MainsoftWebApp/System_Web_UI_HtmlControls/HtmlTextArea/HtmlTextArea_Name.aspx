<%@ Register TagPrefix="cc1" Namespace="GHTWebControls" Assembly="MainsoftWebApp" %>
<%@ Page Language="c#" AutoEventWireup="false" Codebehind="HtmlTextArea_Name.aspx.cs" Inherits="GHTTests.System_Web_dll.System_Web_UI_HtmlControls.HtmlTextArea_Name" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
	<HEAD>
		<title>HtmlTextArea_Name</title>
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
			<P>
				<cc1:GHTSubTest id="GHTSubTest1" runat="server" Width="553px" Height="56px" Description="HtmlTextArea_Name_1">
					<TEXTAREA id="TEXTAREA1" name="TEXTAREA1" rows="2" cols="20" runat="server">Sample Text</TEXTAREA>
				</cc1:GHTSubTest></P>
			<P>
				<cc1:GHTSubTest id="GHTSubTest2" runat="server" Height="56px" Width="553px" Description="HtmlTextArea_Name_2">
					<TEXTAREA id="TEXTAREA2" name="name name" rows="2" cols="20" runat="server">Sample Text</TEXTAREA></cc1:GHTSubTest></P>
			<P>
				<cc1:GHTSubTest id="GHTSubTest3" runat="server" Height="56px" Width="553px" Description="HtmlTextArea_Name_3">
					<TEXTAREA id="TEXTAREA3" name="name_!@#$%^&amp;*()" rows="2" cols="20" runat="server">Sample Text</TEXTAREA></cc1:GHTSubTest></P>
			<P>
				<cc1:GHTSubTest id="GHTSubTest4" runat="server" Height="56px" Width="553px" Description="HtmlTextArea_Name_4">
					<TEXTAREA id="TEXTAREA4" name="" rows="2" cols="20" runat="server">Sample Text</TEXTAREA></cc1:GHTSubTest></P>
			<P>
				<cc1:GHTSubTest id="GHTSubTest5" runat="server" Height="56px" Width="553px" Description="HtmlTextArea_Name_5">
					<TEXTAREA id="TEXTAREA5" name="TEXTAREA5" rows="2" cols="20" runat="server">Sample Text</TEXTAREA></cc1:GHTSubTest></P>
			<P>
				<cc1:GHTSubTest id="GHTSubTest6" runat="server" Height="56px" Width="553px" Description="HtmlTextArea_Name_6">
					<TEXTAREA id="Textarea6" name="TEXTAREA5" rows="2" cols="20" runat="server">Sample Text</TEXTAREA></cc1:GHTSubTest>
				<br>
			</P>
		</form>
	</body>
</HTML>
