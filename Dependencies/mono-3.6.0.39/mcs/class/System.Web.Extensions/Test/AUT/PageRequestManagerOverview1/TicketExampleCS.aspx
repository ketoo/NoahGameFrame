
<%@ Page Language="C#" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<script runat="server">
    protected void ChosenDate_TextChanged(object sender, EventArgs e)
    {
        DateTime dt = new DateTime();
        DateTime.TryParse(ChosenDate.Text, out dt);

        CalendarPicker.SelectedDate = dt;
        CalendarPicker.VisibleDate = dt;
    }
    protected void Close_Click(object sender, EventArgs e)
    {
        SetDateSelectionAndVisible();
    }

    protected void ShowDatePickerPopOut_Click(object sender, ImageClickEventArgs e)
    {
        DatePickerPopOut.Visible = !DatePickerPopOut.Visible;
    }

    protected void CalendarPicker_SelectionChanged(object sender, EventArgs e)
    {
        SetDateSelectionAndVisible();
    }

    private void SetDateSelectionAndVisible()
    {
        if (CalendarPicker.SelectedDates.Count != 0)
            ChosenDate.Text = CalendarPicker.SelectedDate.ToShortDateString();
        DatePickerPopOut.Visible = false;
    }

    protected void SubmitButton_Click(object sender, EventArgs e)
    {
        if (Page.IsValid)
        {
            MessageLabel.Text = "An email with availability was sent.";
        }
        else
        {
            MessageLabel.Text = "";
        }
    }

    protected void Page_Load(object sender, EventArgs e)
    {
        CompareValidatorDate.ValueToCompare = DateTime.Today.ToShortDateString();
        ExtraShow1.Text = DateTime.Today.AddDays(10.0).ToShortDateString();
        ExtraShow2.Text = DateTime.Today.AddDays(11.0).ToShortDateString();
    }

    protected void ExtraShow_Click(object sender, EventArgs e)
    {
        ChosenDate.Text = ((LinkButton)sender).Text;
    }
</script>

<html xmlns="http://www.w3.org/1999/xhtml">
<head id="Head1" runat="server">
    <title>Calendar Example</title>
    <style type="text/css">
        body {
            font-family: Tahoma;
        }
        .PopUpCalendarStyle
        {
              background-color:lightblue;
              position:absolute;
              visibility:show;
              margin: 15px 0px 0px 10px;
              z-index:99;   
              border: solid 2px black;
        }
        .UpdatePanelContainer
        {
            width: 260px;
            height:110px;
        }

    </style>
</head>
<body>
    <form id="form1" runat="server">
        <asp:ScriptManager ID="ScriptManager1" runat="server" />
        <script type="text/javascript">
        Type.registerNamespace("ScriptLibrary");
        ScriptLibrary.BorderAnimation = function(color, duration) {
            this._color = color;
            this._duration = duration;
        }
        ScriptLibrary.BorderAnimation.prototype = {
            animatePanel: function(panelElement) {
                var s = panelElement.style;
                s.borderWidth = '1px';
                s.borderColor = this._color;
                s.borderStyle = 'solid';
                window.setTimeout(
                    function() {{ s.borderWidth = 0; }},
                    this._duration
                );
            }
        }
        ScriptLibrary.BorderAnimation.registerClass('ScriptLibrary.BorderAnimation', null);
        
        var panelUpdatedAnimation = new ScriptLibrary.BorderAnimation('blue', 1000);
        var postbackElement;
        Sys.WebForms.PageRequestManager.getInstance().add_beginRequest(beginRequest);
        Sys.WebForms.PageRequestManager.getInstance().add_pageLoaded(pageLoaded);
        
        function beginRequest(sender, args) {
            postbackElement = args.get_postBackElement();
        }
        function pageLoaded(sender, args) {
            var updatedPanels = args.get_panelsUpdated();
            if (typeof(postbackElement) === "undefined") {
                return;
            } 
            else if (postbackElement.id.toLowerCase().indexOf('extrashow') > -1) {
                for (i=0; i < updatedPanels.length; i++) {            
                    panelUpdatedAnimation.animatePanel(updatedPanels[i]);
                }
            }
        
        }
        </script>
        <h1>Tickets</h1>
        <p>
            <strong>Latest News</strong> Due to overwhelming response, we
            have added two extra shows on:
            <asp:LinkButton ID="ExtraShow1" runat="server" OnClick="ExtraShow_Click" />
            and
            <asp:LinkButton ID="ExtraShow2" runat="server" OnClick="ExtraShow_Click" />.
            Don't forget curtain time is at 7:00pm sharp. No late arrivals.
        </p>
        <hr />
        <div class="UpdatePanelContainer">
            <asp:UpdatePanel runat="server" ID="UpdatePanel1" UpdateMode="Conditional">
                <Triggers>
                    <asp:AsyncPostBackTrigger ControlID="ExtraShow1" />
                    <asp:AsyncPostBackTrigger ControlID="ExtraShow2" />
                </Triggers>
                <ContentTemplate>
                    <fieldset>
                        <legend>Check Ticket Availability</legend>Date
                        <asp:TextBox runat="server" ID="ChosenDate" OnTextChanged="ChosenDate_TextChanged" />
                        <asp:ImageButton runat="server" ID="ShowDatePickerPopOut" OnClick="ShowDatePickerPopOut_Click"
                            ImageUrl="../images/calendar.gif" AlternateText="Choose a date."
                            Height="20px" Width="20px" />
                        <asp:Panel ID="DatePickerPopOut" CssClass="PopUpCalendarStyle"
                            Visible="false" runat="server">
                            <asp:Calendar ID="CalendarPicker" runat="server" OnSelectionChanged="CalendarPicker_SelectionChanged">
                            </asp:Calendar>
                            <br />
                            <asp:LinkButton ID="CloseDatePickerPopOut" runat="server" Font-Size="small"
                                OnClick="Close_Click" ToolTip="Close Pop out">
                            Close
                            </asp:LinkButton>
                        </asp:Panel>
                        <br />
                        Email
                        <asp:TextBox runat="server" ID="EmailTextBox" />
                        <br />
                        <br />
                        <asp:Button ID="SubmitButton" Text="Check" runat="server" ValidationGroup="RequiredFields"
                            OnClick="SubmitButton_Click" />
                        <br />
                        <asp:CompareValidator ID="CompareValidatorDate" runat="server"
                            ControlToValidate="ChosenDate" ErrorMessage="Choose a date in the future."
                            Operator="GreaterThanEqual" Type="Date" Display="None" ValidationGroup="RequiredFields" EnableClientScript="False"></asp:CompareValidator>
                        <asp:RequiredFieldValidator ID="RequiredFieldValidatorDate" runat="server"
                            ControlToValidate="ChosenDate" Display="None" ErrorMessage="Date is required."
                            ValidationGroup="RequiredFields" EnableClientScript="False"></asp:RequiredFieldValidator>
                        <asp:RegularExpressionValidator ID="RegularExpressionValidatorEmail"
                            runat="server" ControlToValidate="EmailTextBox" Display="None"
                            ValidationGroup="RequiredFields" ErrorMessage="The email was not correctly formatted."
                            ValidationExpression="^([0-9a-zA-Z]([-.\w]*[0-9a-zA-Z])*@([0-9a-zA-Z][-\w]*[0-9a-zA-Z]\.)+[a-zA-Z]{2,9})$" EnableClientScript="False"></asp:RegularExpressionValidator>
                        <asp:RequiredFieldValidator ID="RequiredFieldValidatorEmail"
                            runat="server" ValidationGroup="RequiredFields" ControlToValidate="EmailTextBox"
                            Display="None" ErrorMessage="Email is required." EnableClientScript="False"></asp:RequiredFieldValidator><br />
                        <asp:ValidationSummary ID="ValidationSummary1" runat="server"
                            ValidationGroup="RequiredFields" EnableClientScript="False" />
                        <asp:Label ID="MessageLabel" runat="server" />
                    </fieldset>
                </ContentTemplate>
            </asp:UpdatePanel>
        </div>
    </form>
</body>
</html>
