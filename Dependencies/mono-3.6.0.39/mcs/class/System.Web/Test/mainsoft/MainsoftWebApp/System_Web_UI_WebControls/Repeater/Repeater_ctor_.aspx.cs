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
using System.Data;
using System.Collections;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;

namespace GHTTests.System_Web_dll.System_Web_UI_WebControls
{
	public class Repeater_ctor_
		: GHTBaseWeb 
	{
		protected System.Web.UI.WebControls.Repeater Repeater1;
		protected GHTWebControls.GHTSubTest GHTSubTest1;
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

		#region "Template Classes"
		public class t_DBLitTemplate : ITemplate
		{
			public void InstantiateIn(Control container)
			{
				Literal l = new Literal();
				l.Text = "<div>";
				l.DataBinding += new EventHandler(this.BindData);
				container.Controls.Add(l);
			}

			public void BindData(object sender, EventArgs e)
			{
				Literal lc = (Literal)sender;
				RepeaterItem container;
				container = (RepeaterItem)lc.NamingContainer;
				lc.Text = container.DataItem + "</div>";
			}

		}
		public class t_EmptyLitTemplate : ITemplate
		{
			public void InstantiateIn(Control container)
			{
				Literal l = new Literal();
				l.Text = "";
				container.Controls.Add(l);
			}

		}
		public class t_PlainTextLitTemplate : ITemplate
		{
			public void InstantiateIn(Control container)
			{
				Literal l = new Literal();
				l.Text = "Plain text template create at run time";
				container.Controls.Add(l);
			}

		}
		public class t_HtmlLitTemplate : ITemplate
		{
			public void InstantiateIn(Control container)
			{
				Literal l = new Literal();
				l.Text = "<div><b><i>Html template created at run time</i></b></div>";
				container.Controls.Add(l);
			}

		}
		public class t_ControlLitTemplate : ITemplate
		{
			public void InstantiateIn(Control container)
			{
				HtmlInputButton l = new HtmlInputButton();
				l.Value = "Control template";
				container.Controls.Add(l);
			}

		}
		#endregion

		private void Page_Load(object sender, System.EventArgs e) 
		{
			//Put user code to initialize the page here

			System.Web.UI.HtmlControls.HtmlForm frm = (HtmlForm)this.FindControl("Form1");
			GHTTestBegin(frm);

			GHTActiveSubTest = GHTSubTest1;
			try 
			{
				Repeater1.DataSource = GHTTests.GHDataSources.DSArray();
				Repeater1.DataBind();
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}

			GHTSubTestBegin("Code base constructor 1");
			try 
			{
				System.Web.UI.WebControls.Repeater Repeater2 = new System.Web.UI.WebControls.Repeater();
				GHTActiveSubTest.Controls.Add(Repeater2);
				Repeater2.DataSource = GHTTests.GHDataSources.DSArrayList();
				Repeater2.ItemTemplate = new t_DBLitTemplate();
				Repeater2.DataBind();
			}
			catch (Exception ex) 
			{
				GHTSubTestUnexpectedExceptionCaught(ex);
			}
			GHTSubTestEnd();

			GHTTestEnd();
		}
	}
}
