//
// System.Web.UI.FileLevelControlBuilder.cs
//
// Authors:
//     Arina Itkes (arinai@mainsoft.com)
//     Marek Habersack <mhabersack@novell.com>
//
// (C) 2007 Mainsoft Co. (http://www.mainsoft.com)
// (C) 2009-2010 Novell, Inc (http://novell.com/)
//
//
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

using System.Web.UI;
using System.Web.UI.WebControls;

namespace System.Web.UI
{
	public class FileLevelPageControlBuilder : RootBuilder
	{
		bool hasContentControls;
		bool hasLiteralControls;
		bool hasOtherControls;
		
		public FileLevelPageControlBuilder ()
		{
		}
		
		public override void AppendLiteralString (string text)
		{
			bool emptyText = text != null ? text.Trim ().Length == 0 : true;
			if (hasContentControls && !emptyText)
				throw new HttpException ("Literal strings cannot be appended to Content pages.");

			if (!emptyText)
				hasLiteralControls = true;
			
			base.AppendLiteralString (text);
		}
		
		public override void AppendSubBuilder (ControlBuilder subBuilder)
		{
			if (subBuilder == null) {
				base.AppendSubBuilder (subBuilder);
				return;
			}
			
			if (typeof (ContentBuilderInternal).IsAssignableFrom (subBuilder.GetType ())) {
				if (hasOtherControls)
					throw new HttpException ("Only Content controls are supported on content pages.");
				
				hasContentControls = true;
				if (hasLiteralControls)
					throw new HttpParseException ("Only Content controls are supported on content pages.");
			} else
				hasOtherControls = true;
			
			base.AppendSubBuilder (subBuilder);
		}
	}
}

