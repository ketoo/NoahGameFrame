<%@ Page Language="C#" AutoEventWireup="true" CodeFile="MyPage.aspx.cs" Inherits="MyPage" %>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Untitled Page</title>
</head>
<body>
<script runat="server">

    protected void Page_Load (object sender, EventArgs e)
    {
        DetailsView1.DataKeyNames = new string[] { "ID", "FName", "LName" };
        MonoTests.System.Web.UI.WebControls.TableObject.ds = MonoTests.System.Web.UI.WebControls.TableObject.CreateDataTable (); 
    }
</script>
    <form id="form1" runat="server">
    test1
    <div>
        <%= MonoTests.stand_alone.WebHarness.HtmlDiff.BEGIN_TAG %><asp:DetailsView ID="DetailsView1" runat="server" AllowPaging="True" DataSourceID="ObjectDataSource1" HeaderText ="Using Header Text property"
            Height="50px" Width="125px">
            <Fields>
                <asp:CommandField ShowDeleteButton="True" />
            </Fields>
            <HeaderTemplate>
                Header Template<asp:Button ID="Button1" runat="server" Text ="Header button" />
            </HeaderTemplate>
        </asp:DetailsView><%= MonoTests.stand_alone.WebHarness.HtmlDiff.END_TAG %>
        <asp:ObjectDataSource ID="ObjectDataSource1" runat="server" DeleteMethod="Delete"
            SelectMethod="GetMyData" TypeName="MonoTests.System.Web.UI.WebControls.TableObject">
            <DeleteParameters>
                <asp:Parameter Name="ID" Type="String" />
                <asp:Parameter Name="FName" Type="String" />
                <asp:Parameter Name="LName" Type="String" />
            </DeleteParameters>
        </asp:ObjectDataSource>
    </div>
    </form>
</body>
</html>
