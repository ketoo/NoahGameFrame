<%@ Page Language="C#" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<script runat="server">
    protected void Button1_Click(object sender, EventArgs e)
    {
        // Introducing delay for demonstration.
        System.Threading.Thread.Sleep(3000);
        Label1.Text = "Page refreshed at " +
            DateTime.Now.ToString();       
    }
</script>

<html xmlns="http://www.w3.org/1999/xhtml" >
<head id="Head1" runat="server">
    <title>UpdateProgress Tutorial</title>
    <style type="text/css">
    #UpdatePanel1 { 
      width:300px; height:100px;
     }
    </style>
</head>
<body>
    <form id="form1" runat="server">
    <div>
        <asp:ScriptManager ID="ScriptManager1" runat="server">
        </asp:ScriptManager>
        <asp:UpdatePanel ID="UpdatePanel1" runat="server">
            <ContentTemplate>
                <fieldset>
                <legend>UpdatePanel</legend>
                <asp:Label ID="Label1" runat="server" Text="Initial page rendered."></asp:Label><br />
                <asp:Button ID="Button1" runat="server" Text="Button" OnClick="Button1_Click" />
                </fieldset>
            </ContentTemplate>
        </asp:UpdatePanel>
        <asp:UpdateProgress ID="UpdateProgress1" runat="server">
            <ProgressTemplate>
                Processing...
            </ProgressTemplate>
        </asp:UpdateProgress>
    
    </div>
    </form>
</body>
</html>
