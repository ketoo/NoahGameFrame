//
// System.Web.UI.HtmlControls.HtmlInputFile.cs
//
// Author:
//	Dick Porter  <dick@ximian.com>
//
// Copyright (C) 2005-2010 Novell, Inc (http://www.novell.com)
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

using System.Collections.Specialized;
using System.ComponentModel;
using System.Security.Permissions;

namespace System.Web.UI.HtmlControls 
{
	// CAS
	[AspNetHostingPermission (SecurityAction.LinkDemand, Level = AspNetHostingPermissionLevel.Minimal)]
	[AspNetHostingPermission (SecurityAction.InheritanceDemand, Level = AspNetHostingPermissionLevel.Minimal)]
	// attributes
	[ValidationProperty ("Value")]
	public class HtmlInputFile : HtmlInputControl , IPostBackDataHandler
	{
		HttpPostedFile posted_file;

		public HtmlInputFile () : base ("file")
		{
		}

		[DefaultValue ("")]
		[DesignerSerializationVisibility (DesignerSerializationVisibility.Hidden)]
		[WebSysDescription("")]
		public string Accept {
			get {
				string acc = Attributes["accept"];

				if (acc == null) {
					return (String.Empty);
				}

				return (acc);
			}
			set {
				if (value == null) {
					Attributes.Remove ("accept");
				} else {
					Attributes["accept"] = value;
				}
			}
		}

		[DefaultValue ("")]
		[DesignerSerializationVisibility (DesignerSerializationVisibility.Hidden)]
		[WebSysDescription("")]
		public int MaxLength {
			get {
				string maxlen = Attributes["maxlength"];
				
				if (maxlen == null) {
					return (-1);
				} else {
					return (Convert.ToInt32 (maxlen));
				}
			}
			set {
				if (value == -1) {
					Attributes.Remove ("maxlength");
				} else {
					Attributes["maxlength"] = value.ToString ();
				}
			}
		}
		
		[DefaultValue ("")]
		[DesignerSerializationVisibility (DesignerSerializationVisibility.Hidden)]
		[WebSysDescription("")]
		[WebCategory("Misc")]
		public HttpPostedFile PostedFile {
			get {
				return (posted_file);
			}
		}
		
		[DefaultValue ("-1")]
		[DesignerSerializationVisibility (DesignerSerializationVisibility.Hidden)]
		[WebSysDescription("")]
		[WebCategory("Appearance")]
		public int Size {
			get {
				string size = Attributes["size"];
				
				if (size == null) {
					return (-1);
				} else {
					return (Convert.ToInt32 (size));
				}
			}
			set {
				if (value == -1) {
					Attributes.Remove ("size");
				} else {
					Attributes["size"] = value.ToString ();
				}
			}
		}
		
		[Browsable (false)]
		public override string Value {
			get {
				HttpPostedFile file = PostedFile;
				if (file == null)
					return string.Empty;

				return file.FileName;
			}
			set {
				throw new NotSupportedException ("The value property on HtmlInputFile is not settable.");
			}
		}

		protected internal override void OnPreRender (EventArgs e)
		{
			base.OnPreRender (e);

			Page page = Page;
			if (page != null && !Disabled) {
				page.RegisterRequiresPostBack (this);
				page.RegisterEnabledControl (this);
			}
			
			HtmlForm form = (HtmlForm) SearchParentByType (typeof (HtmlForm));
			if (form != null && form.Enctype == String.Empty)
				form.Enctype = "multipart/form-data";
		}

		Control SearchParentByType (Type type)
		{
			Control ctrl = Parent;
			while (ctrl != null) {
				if (type.IsAssignableFrom (ctrl.GetType ())) {
					return ctrl;
				}
				ctrl = ctrl.Parent;
			}

			return null;
		}

		bool LoadPostDataInternal (string postDataKey, NameValueCollection postCollection)
		{
			Page page = Page;
			if (page != null)
				posted_file = page.Request.Files [postDataKey];
			
			return (false);
		}

		void RaisePostDataChangedEventInternal ()
		{
			/* No events to raise */
		}

		protected virtual bool LoadPostData (string postDataKey, NameValueCollection postCollection)
		{
			return LoadPostDataInternal (postDataKey, postCollection);
		}

		protected virtual void RaisePostDataChangedEvent ()
		{
			RaisePostDataChangedEventInternal ();
		}
		
		bool IPostBackDataHandler.LoadPostData (string postDataKey, NameValueCollection postCollection)
		{
			return LoadPostData (postDataKey, postCollection);
		}

		void IPostBackDataHandler.RaisePostDataChangedEvent ()
		{
			RaisePostDataChangedEvent ();
		}
	}
}

	
