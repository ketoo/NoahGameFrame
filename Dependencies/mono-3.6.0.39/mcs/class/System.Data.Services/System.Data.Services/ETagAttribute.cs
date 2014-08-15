﻿//
// ETagAttribute.cs
//
// Author:
//   Eric Maupin  <me@ermau.com>
//
// Copyright (c) 2009 Eric Maupin (http://www.ermau.com)
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

using System.Collections.ObjectModel;

namespace System.Data.Services {
	[AttributeUsage (AttributeTargets.Class, AllowMultiple = false, Inherited = true )]
	public sealed class ETagAttribute : Attribute {
		public ETagAttribute (string propertyName)
		{
			if (propertyName == null)
				throw new ArgumentNullException ("propertyName");

			this.properties = new ReadOnlyCollection<string> (new[] { propertyName });
		}

		public ETagAttribute (params string[] propertyNames)
		{
			if (propertyNames == null)
				throw new ArgumentNullException ("propertyNames");

			this.properties = new ReadOnlyCollection<string> (propertyNames);
		}

		public ReadOnlyCollection<string> PropertyNames {
			get { return this.properties; }
		}

		private readonly ReadOnlyCollection<string> properties;
	}
}
