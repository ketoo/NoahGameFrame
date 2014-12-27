<%@ Page Language="C#" %>
<%@ Register Src="Controls/WebUserControl.ascx" TagName="WebUserControl" TagPrefix="uc1" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<script runat="server">
    
    protected void WebUserControl1_Click(object sender, EventArgs e)
    {
        if (WebUserControl1.Range != -1)
            Label1.Text = "You selected " + 
                WebUserControl1.Range.ToString() + 
                " day(s).";
        CalendarPanel.Visible = false;
    }

    protected void ShowCalendar(object sender, EventArgs e)
    {
        CalendarPanel.Visible = true;
    }

    protected void Page_Load(object sender, EventArgs e)
    {
        if (!IsPostBack)
        {
            CalendarPanel.Visible = false;
            Label1.Text = "You have not selected any days.";
        }
    }
</script>
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>UpdatePanel UserControl Example</title>
    <style type="text/css">
    div.CalendarPanel
    {
      background-color: white;
      text-align: center;
      position: absolute;
      top: 30px;
      left: 160px;
      width: 300px;
    }
	</style>
</head>
<body>
    <form id="form1" runat="server">
        <div>

<!-- This example is not meant to be shown to users as is.
     it is providing snippets for conceptual topic. -->
     

            <asp:ScriptManager ID="TheScriptManager" 
                               runat="server" />
                <asp:UpdatePanel ID="UpdatePanel1" 
                                 UpdateMode="Conditional" 
                                 runat="server">
                     <ContentTemplate>
                     </ContentTemplate>
                </asp:UpdatePanel>


            <asp:LinkButton ID="ShowCalendarLinkButton" 
                            Text="Choose a number of days"
                            OnClick="ShowCalendar"
                            runat="server" />
            <asp:Panel ID="CalendarPanel" 
                       GroupingText="Choose a Range" 
                       CssClass="CalendarPanel"
                       runat="server">
            </asp:Panel>

            <uc1:WebUserControl ID="WebUserControl1"
                                OnInnerClick="WebUserControl1_Click"
                                runat="server">
            </uc1:WebUserControl>

            <br />

            <asp:Label ID="Label1" 
                       runat="server"/>
        </div>
    </form>
</body>
</html>
