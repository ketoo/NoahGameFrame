﻿<%@ Page Language="C#" AutoEventWireup="true" CodeFile="search.aspx.cs" Inherits="search" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
	<%= AppDomain.CurrentDomain.GetData ("BEGIN_CODE_MARKER") %>Search term is: <asp:Label runat="server" ID="label1" /><br />
	Search term from expression is: <asp:Label ID="label2" runat="server" Text="<%$RouteValue:SearchTerm%>" /><br />
	<pre runat="server" id="testLog" /><%= AppDomain.CurrentDomain.GetData ("END_CODE_MARKER") %>
    </div>
    </form>
</body>
</html>
