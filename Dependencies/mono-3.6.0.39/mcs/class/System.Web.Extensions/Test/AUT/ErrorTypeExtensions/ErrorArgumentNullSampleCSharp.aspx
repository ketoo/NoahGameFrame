
<%@ Page Language="C#" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head id="Head1" runat="server">
    <title>Samples</title>
</head>
<body>
    <form id="form1" runat="server">
        <asp:ScriptManager runat="server" ID="ScriptManager01">
            <Scripts>
               <asp:ScriptReference Name="ErrorSampleJS.js" Assembly="ErrorSample" />
            </Scripts>
        </asp:ScriptManager>
    </form>
</body>
</html>
