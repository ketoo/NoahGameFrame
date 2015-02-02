//
// Authors:
//   Rafael Mizrahi   <rafim@mainsoft.com>
//   Erez Lotan       <erezl@mainsoft.com>
//   Vladimir Krasnov <vladimirk@mainsoft.com>
//
//
// Copyright (c) 2002-2005 Mainsoft Corporation.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

using System;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;

namespace GHTTests.System_Web_dll.System_Web_UI_WebControls
{
    public class WebControl_Font
        : GHTWebControlBase
	{
		#region Web Form Designer generated code
		override protected void OnInit(EventArgs e) {
			//
			// CODEGEN: This call is required by the ASP.NET Web Form Designer.
			//
			InitializeComponent();
			base.OnInit(e);
		}
		
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {    
			this.Load += new System.EventHandler(this.Page_Load);
		}
		#endregion

        private void Page_Load(object sender, System.EventArgs e) {
            HtmlForm frm  = (HtmlForm)FindControl("Form1");
            GHTTestBegin(frm);
			foreach (Type currentType in TypesToTest)
			{
				GHTHeader(currentType.ToString());
				Test(currentType);
			}
			GHTTestEnd();
        }

		private void Test(Type ctrlType)
		{
			try
			{
				this.GHTSubTestBegin(ctrlType, "Bold");
				this.TestedControl.Font.Bold = true;
				this.GHTSubTestAddResult(this.TestedControl.Font.ToString());
			}
			catch (Exception exception9)
			{
				// ProjectData.SetProjectError(exception9);
				Exception exception1 = exception9;
				this.GHTSubTestUnexpectedExceptionCaught(exception1);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "Italic");
				this.TestedControl.Font.Italic = true;
				this.GHTSubTestAddResult(this.TestedControl.Font.ToString());
			}
			catch (Exception exception10)
			{
				// ProjectData.SetProjectError(exception10);
				Exception exception2 = exception10;
				this.GHTSubTestUnexpectedExceptionCaught(exception2);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "Name ");
				this.TestedControl.Font.Name = "Lucida Console";
			}
			catch (Exception exception11)
			{
				// ProjectData.SetProjectError(exception11);
				Exception exception3 = exception11;
				this.GHTSubTestUnexpectedExceptionCaught(exception3);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "Names");
				string[] textArray1 = new string[] { "verdana", "Lucida Console", "Courier" } ;
				this.TestedControl.Font.Names = textArray1;
			}
			catch (Exception exception12)
			{
				// ProjectData.SetProjectError(exception12);
				Exception exception4 = exception12;
				this.GHTSubTestUnexpectedExceptionCaught(exception4);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "Overline");
				this.TestedControl.Font.Overline = true;
				this.GHTSubTestAddResult(this.TestedControl.Font.ToString());
			}
			catch (Exception exception13)
			{
				// ProjectData.SetProjectError(exception13);
				Exception exception5 = exception13;
				this.GHTSubTestUnexpectedExceptionCaught(exception5);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "FontSize");
				FontUnit unit1 = new FontUnit(FontSize.XXLarge);
				this.TestedControl.Font.Size = unit1;
				this.GHTSubTestAddResult(this.TestedControl.Font.ToString());
			}
			catch (Exception exception14)
			{
				// ProjectData.SetProjectError(exception14);
				Exception exception6 = exception14;
				this.GHTSubTestUnexpectedExceptionCaught(exception6);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "Strikeout");
				this.TestedControl.Font.Strikeout = true;
				this.GHTSubTestAddResult(this.TestedControl.Font.ToString());
			}
			catch (Exception exception15)
			{
				// ProjectData.SetProjectError(exception15);
				Exception exception7 = exception15;
				this.GHTSubTestUnexpectedExceptionCaught(exception7);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "Underline");
				this.TestedControl.Font.Underline = true;
				this.GHTSubTestAddResult(this.TestedControl.Font.ToString());
			}
			catch (Exception exception16)
			{
				// ProjectData.SetProjectError(exception16);
				Exception exception8 = exception16;
				this.GHTSubTestUnexpectedExceptionCaught(exception8);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
		}
 
	}
}
