//
// System.Web.UI.ThemeProvider.cs
//
// Authors:
//   Chris Toshok (toshok@ximian.com)
//
// (C) 2006-2010 Novell, Inc (http://www.novell.com)
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

using System.Collections;
using System.Collections.Specialized;
using System.ComponentModel;
using System.ComponentModel.Design;

namespace System.Web.UI 
{
	public sealed class ThemeProvider
	{
		IDesignerHost host;
		string name;
		string[] cssFiles;
		//string themeDefinition;
		//string themePath;

		public ThemeProvider (IDesignerHost host,
				      string name,
				      string themeDefinition,
				      string[] cssFiles,
				      string themePath)
		{
			this.host = host;
			this.name = name;
			//this.themeDefinition = themeDefinition;
			this.cssFiles = cssFiles; /* XXX copy? */
			//this.themePath = themePath;
		}

		[MonoTODO("Not implemented")]
		public SkinBuilder GetSkinBuilder (Control control)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO ("Not implemented")]
		public IDictionary GetSkinControlBuildersForControlType (Type type)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO ("Not implemented")]
		public ICollection GetSkinsForControl (Type type)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO ("Not implemented")]
		public int ContentHashCode {
			get { throw new NotImplementedException (); }
		}

		public ICollection CssFiles {
			get { return cssFiles; }
		}

		public IDesignerHost DesignerHost {
			get { return host; }
		}

		public string ThemeName {
			get { return name; }
		}
	}

}
