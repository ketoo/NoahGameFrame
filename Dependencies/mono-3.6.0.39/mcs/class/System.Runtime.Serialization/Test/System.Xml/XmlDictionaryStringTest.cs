//
// XmlDictionaryStringTest.cs
//
// Author:
//   Jonathan Pryor <jpryor@novell.com>
//
// Copyright (C) 2009 Novell, Inc.  http://www.novell.com
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
using System.Xml;
using NUnit.Framework;

namespace MonoTests.System.Xml
{
	[TestFixture]
	public class XmlDictionaryStringTest
	{
		[Test, ExpectedException (typeof (ArgumentNullException))]
		public void Constructor_DictionaryNull ()
		{
			new XmlDictionaryString (null, "foo", -1);
		}

		[Test, ExpectedException (typeof (ArgumentNullException))]
		public void Constructor_ValueyNull ()
		{
			new XmlDictionaryString (new XmlDictionary (), null, -1);
		}

		[Test, ExpectedException (typeof (ArgumentOutOfRangeException))]
		public void Constructor_KeyNegative ()
		{
			new XmlDictionaryString (new XmlDictionary (), "key", -1);
		}

		[Test, ExpectedException (typeof (ArgumentOutOfRangeException))]
		public void Constructor_KeyTooLarge ()
		{
			new XmlDictionaryString (new XmlDictionary (), "key", (int.MaxValue/4)+1);
		}
	}
}
