﻿// 
// ExpandedWrapper`1.cs
//  
// Author:
//       Marek Habersack <grendel@twistedcode.net>
// 
// Copyright (c) 2011 Novell, Inc
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

using System;
using System.ComponentModel;
using System.Data.Services;
using System.Data.Services.Internal;
using System.Runtime;

namespace System.Data.Services.Internal
{
	[EditorBrowsable (EditorBrowsableState.Never)]
	public abstract class ExpandedWrapper <TExpandedElement> : IExpandedResult
	{
		public string Description {
			get { throw new NotImplementedException (); }
			set { throw new NotImplementedException (); }
		}

		public TExpandedElement ExpandedElement {
			get { throw new NotImplementedException (); }
			set { throw new NotImplementedException (); }
		}

		object IExpandedResult.ExpandedElement {
			get { throw new NotImplementedException (); }
		}

		public object GetExpandedPropertyValue (string name)
		{
			throw new NotImplementedException ();
		}

		protected abstract object InternalGetExpandedPropertyValue (int nameIndex);

		protected ExpandedWrapper ()
		{
			throw new NotImplementedException ();
		}
	}
}
