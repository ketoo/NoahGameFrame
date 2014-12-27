//
// System.Web.UI.WebControls.GridViewUpdatedEventArgs.cs
//
// Authors:
//   Sanjay Gupta (gsanjay@novell.com)
//
// (C) 2004-2010 Novell, Inc (http://www.novell.com)
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

using System.Collections.Specialized;

namespace System.Web.UI.WebControls
{
	public class GridViewUpdatedEventArgs : EventArgs
	{
		int rowsAffected;
		Exception e;
		bool exceptionHandled;
		bool keepEditMode;
		IOrderedDictionary keys;
		IOrderedDictionary newValues;
		IOrderedDictionary oldValues;
		
		public GridViewUpdatedEventArgs (int affectedRows, Exception e)
		{
			this.rowsAffected = affectedRows;
			this.e = e;
			this.exceptionHandled = false;
			this.keepEditMode = false;
		}
		
		internal GridViewUpdatedEventArgs (int affectedRows, Exception e, IOrderedDictionary keys, IOrderedDictionary oldValues, IOrderedDictionary newValues)
			: this (affectedRows, e)
		{
			this.keys = keys;
			this.newValues = newValues;
			this.oldValues = oldValues;
		}
		
		public int AffectedRows {
			get { return rowsAffected; }
		}

		public Exception Exception {
			get { return e; }
		}

		public bool ExceptionHandled {
			get { return exceptionHandled; }
			set { exceptionHandled = value; }
		}
	
		public bool KeepInEditMode {
			get { return keepEditMode; }
			set { keepEditMode = value; }
		}

		public IOrderedDictionary Keys {
			get { return keys; }
		}

		public IOrderedDictionary NewValues {
			get { return newValues; }
		}

		public IOrderedDictionary OldValues {
			get { return oldValues; }
		}
	}
}

