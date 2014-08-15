// 
// System.Web.ParserErrorCollection.cs
//
// Authors:
//	Lluis Sanchez Gual (lluis@novell.com)
//
// (C) 2004-2009 Novell, Inc (http://www.novell.com)
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

using System.Collections;
using System.Runtime.Serialization;

namespace System.Web
{
	[Serializable]
	public sealed class ParserErrorCollection: CollectionBase
	{
		public ParserErrorCollection ()
		{
		}

		public ParserErrorCollection (ParserError[] errors)
		{
			InnerList.AddRange (errors);
		}
		
		public ParserError this [int index]
		{
			get { return (ParserError) InnerList [index]; }
			set { InnerList[index] = value; }
		}
		
		public int Add (ParserError error)
		{
			return List.Add (error);
		}
		
		public void AddRange (ParserErrorCollection collection)
		{
			InnerList.AddRange (collection);
		}
		
		public void AddRange (ParserError[] errors)
		{
			InnerList.AddRange (errors);
		}
		
		public bool Contains (ParserError error)
		{
			return InnerList.Contains (error);
		}
		
		public void CopyTo (ParserError[] errors, int index)
		{
			List.CopyTo (errors, index);
		}
		
		public int IndexOf (ParserError error)
		{
			return InnerList.IndexOf (error);
		}
		
		public void Insert (int index, ParserError error)
		{
			InnerList.Insert (index, error);
		}
		
		public void Remove (ParserError error)
		{
			InnerList.Remove (error);
		}
	}
}

