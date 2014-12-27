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
using System.Collections;

namespace GHTTests.System_Web_dll.System_Web_UI_WebControls
{
    public class WebControl_TableStyle_BackImageUrl
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

        //Overrides GHTWebControlBase.InitTypes
        //This method initializes the types that will be contained in the TypesToTest array.
        //This only controls that TableStyle is relevant for, will be tested.
		protected override void InitTypes()
		{
			base.m_derivedTypes = new ArrayList();
			base.m_derivedTypes.Add(typeof(RadioButtonList));
			base.m_derivedTypes.Add(typeof(CheckBoxList));
			base.m_derivedTypes.Add(typeof(DataGrid));
			base.m_derivedTypes.Add(typeof(DataList));
			base.m_derivedTypes.Add(typeof(Table));
		}
 


        private void Page_Load(object sender, System.EventArgs e) 
		{
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
				this.GHTSubTestBegin(ctrlType, "Relative path");
				TableStyle style1 = (TableStyle) this.TestedControl.ControlStyle;
				style1.BackImageUrl = "cat.jpg";
			}
			catch (Exception exception8)
			{
				// ProjectData.SetProjectError(exception8);
				Exception exception1 = exception8;
				this.GHTSubTestUnexpectedExceptionCaught(exception1);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "absolute path");
				TableStyle style2 = (TableStyle) this.TestedControl.ControlStyle;
				style2.BackImageUrl = "http://localhost:/System_Web_dll/System_Web_UI_WebControls/WebControl/cat.jpg";
			}
			catch (Exception exception9)
			{
				// ProjectData.SetProjectError(exception9);
				Exception exception2 = exception9;
				this.GHTSubTestUnexpectedExceptionCaught(exception2);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "Defied only in CssClass");
				TableStyle style3 = (TableStyle) this.TestedControl.ControlStyle;
				style3.CssClass = "CssClass1";
			}
			catch (Exception exception10)
			{
				// ProjectData.SetProjectError(exception10);
				Exception exception3 = exception10;
				this.GHTSubTestUnexpectedExceptionCaught(exception3);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "Defied only in CssClass");
				TableStyle style4 = (TableStyle) this.TestedControl.ControlStyle;
				style4.CssClass = "CssClass1";
				style4.BackImageUrl = "http://localhost:/System_Web_dll/System_Web_UI_WebControls/WebControl/dog.jpg";
			}
			catch (Exception exception11)
			{
				// ProjectData.SetProjectError(exception11);
				Exception exception4 = exception11;
				this.GHTSubTestUnexpectedExceptionCaught(exception4);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "Default value");
				TableStyle style5 = (TableStyle) this.TestedControl.ControlStyle;
				this.GHTSubTestAddResult(style5.BackImageUrl);
			}
			catch (Exception exception12)
			{
				// ProjectData.SetProjectError(exception12);
				Exception exception5 = exception12;
				this.GHTSubTestUnexpectedExceptionCaught(exception5);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
			try
			{
				this.GHTSubTestBegin(ctrlType, "Throws exception on null value");
				TableStyle style6 = (TableStyle) this.TestedControl.ControlStyle;
				style6.BackImageUrl = null;
				this.GHTSubTestExpectedExceptionNotCaught("ArgumentNullException");
			}
			catch (ArgumentNullException exception13)
			{
				// ProjectData.SetProjectError(exception13);
				ArgumentNullException exception6 = exception13;
				this.GHTSubTestExpectedExceptionCaught(exception6);
				// ProjectData.ClearProjectError();
			}
			catch (Exception exception14)
			{
				// ProjectData.SetProjectError(exception14);
				Exception exception7 = exception14;
				this.GHTSubTestUnexpectedExceptionCaught(exception7);
				// ProjectData.ClearProjectError();
			}
			this.GHTSubTestEnd();
		}
 
	}
}
