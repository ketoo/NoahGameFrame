<%@ Register TagPrefix="cc1" Namespace="GHTWebControls" Assembly="MainsoftWebApp" %>
<%@ Page Language="c#" AutoEventWireup="false" Codebehind="HtmlInputImage_CausesValidation.aspx.cs" Inherits="GHTTests.System_Web_dll.System_Web_UI_HtmlControls.HtmlInputImage_CausesValidation" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
	<HEAD>
		<title>HtmlInputImage_CausesValidation</title>
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
				<cc1:GHTSubTest id="GHTSubTest1" runat="server" Height="56px" Width="553px" Description="HtmlInputImage_CausesValidation_1">
					<INPUT id="Image1" type="image" src="/test.img" value="value of ImageInput1" name="InputImage1"
						runat="server">
				</cc1:GHTSubTest></P>
			<P>
				<cc1:GHTSubTest id="GHTSubTest2" runat="server" Height="56px" Width="553px" Description="HtmlInputImage_CausesValidation_2">
					<INPUT id="Image2" type="image" src="/test.img" value="value of ImageInput2" name="InputImage2"
						runat="server">
				</cc1:GHTSubTest></P>
		</form>
		<br>
		<br>
	</body>
</HTML>
