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
	public class DataList_RepeatColumns
		: GHTBaseWeb 
	{
		#region Web Form Designer generated code
		override protected void OnInit(EventArgs e) 
		{
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
		private void InitializeComponent() 
		{    
			this.Load += new System.EventHandler(this.Page_Load);

		}
		#endregion

		protected System.Web.UI.WebControls.DataList DataList1;
		protected GHTWebControls.GHTSubTest GHTSubTest1;
		protected System.Web.UI.WebControls.DataList DataList2;
		protected GHTWebControls.GHTSubTest GHTSubTest2;
		protected System.Web.UI.WebControls.DataList DataList3;
		protected GHTWebControls.GHTSubTest GHTSubTest3;
		protected System.Web.UI.WebControls.DataList DataList6;
		protected GHTWebControls.GHTSubTest GHTSubTest6;
		protected System.Web.UI.WebControls.DataList DataList4;
		protected GHTWebControls.GHTSubTest GHTSubTest4;
		protected System.Web.UI.WebControls.DataList DataList5;
		protected GHTWebControls.GHTSubTest GHTSubTest5;
		protected System.Web.UI.WebControls.DataList DataList7;
		protected GHTWebControls.GHTSubTest GHTSubTest7;
		protected System.Web.UI.WebControls.DataList DataList8;
		protected GHTWebControls.GHTSubTest GHTSubTest8;
		protected System.Web.UI.WebControls.DataList DataList9;
		protected GHTWebControls.GHTSubTest GHTSubTest9;
		protected System.Web.UI.WebControls.DataList DataList12;
		protected GHTWebControls.GHTSubTest GHTSubTest12;
		protected System.Web.UI.WebControls.DataList DataList10;
		protected GHTWebControls.GHTSubTest GHTSubTest10;
		protected System.Web.UI.WebControls.DataList DataList11;
		protected GHTWebControls.GHTSubTest GHTSubTest11;
		protected static string [] m_data = new String[] {"aaa", "bbb", "ccc", "ddd", "eee", "fff", "ggg"};

		private void Page_Load(object sender, System.EventArgs e) 
		{
			HtmlForm frm  = (HtmlForm)FindControl("Form1");
			GHTTestBegin(frm);

			GHTActiveSubTest = GHTSubTest1;
			try 
			{
				DataList1.DataBind();;
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTActiveSubTest = GHTSubTest2;
			try 
			{
				DataList2.DataBind();
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTActiveSubTest = GHTSubTest3;
			try 
			{
				DataList3.DataBind();
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTActiveSubTest = GHTSubTest4;
			try 
			{
				DataList4.DataBind();
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTActiveSubTest = GHTSubTest5;
			try 
			{
				DataList5.DataBind();
				DataList5.RepeatColumns = -1;
				GHTSubTestExpectedExceptionNotCaught("ArgumentOutOfRangeException");
			}
			catch (ArgumentOutOfRangeException ex)
			{
				GHTSubTestExpectedExceptionCaught(ex);
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTActiveSubTest = GHTSubTest6;
			try 
			{
				DataList6.DataBind();
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTActiveSubTest = GHTSubTest7;
			try 
			{
				DataList7.DataBind();
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTActiveSubTest = GHTSubTest8;
			try 
			{
				DataList8.DataBind();
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTActiveSubTest = GHTSubTest9;
			try 
			{
				DataList9.DataBind();
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTActiveSubTest = GHTSubTest10;
			try 
			{
				DataList10.DataBind();
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTActiveSubTest = GHTSubTest11;
			try 
			{
				DataList11.DataBind();
				DataList11.RepeatColumns = -1;
				GHTSubTestExpectedExceptionNotCaught("ArgumentOutOfRangeException");
			}
			catch (ArgumentOutOfRangeException ex)
			{
				GHTSubTestExpectedExceptionCaught(ex);
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTActiveSubTest = GHTSubTest12;
			try 
			{
				DataList12.DataBind();
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTTestEnd();
		}
	}
}
