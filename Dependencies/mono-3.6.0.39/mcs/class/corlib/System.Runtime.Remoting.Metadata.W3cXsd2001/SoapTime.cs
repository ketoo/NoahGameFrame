//
// System.Runtime.Remoting.Metadata.W3cXsd2001.SoapTime
//
// Authors:
//      Martin Willemoes Hansen (mwh@sysrq.dk)
//      Lluis Sanchez Gual (lluis@ximian.com)
//
// (C) 2003 Martin Willemoes Hansen
//

//
// Copyright (C) 2004 Novell, Inc (http://www.novell.com)
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
using System.Globalization;

namespace System.Runtime.Remoting.Metadata.W3cXsd2001 
{
	[Serializable]
	[System.Runtime.InteropServices.ComVisible (true)]
	public sealed class SoapTime : ISoapXsd
	{
		static readonly string[] _datetimeFormats = new string[]
		{
			"HH:mm:ss",
			"HH:mm:ss.f",
			"HH:mm:ss.ff",
			"HH:mm:ss.fff",
			"HH:mm:ss.ffff",
			"HH:mm:ss.fffff",
			"HH:mm:ss.ffffff",
			"HH:mm:ss.fffffff",
			"HH:mm:sszzz",
			"HH:mm:ss.fzzz",
			"HH:mm:ss.ffzzz",
			"HH:mm:ss.fffzzz",
			"HH:mm:ss.ffffzzz",
			"HH:mm:ss.fffffzzz",
			"HH:mm:ss.ffffffzzz",
			"HH:mm:ss.fffffffzzz",
			"HH:mm:ssZ",
			"HH:mm:ss.fZ",
			"HH:mm:ss.ffZ",
			"HH:mm:ss.fffZ",
			"HH:mm:ss.ffffZ",
			"HH:mm:ss.fffffZ",
			"HH:mm:ss.ffffffZ",
			"HH:mm:ss.fffffffZ"
		};
		
		DateTime _value;
		
		public SoapTime ()
		{
		}

		public SoapTime (DateTime value)
		{
			_value = value;
		}

		public DateTime Value {
			get { return _value; } 
			set { _value = value; }
		}
		
		public static string XsdType {
			get { return "time"; }
		}

		public string GetXsdType()
		{
			return XsdType;
		}
		
		public static SoapTime Parse (string value)
		{
			return new SoapTime (DateTime.ParseExact (value, _datetimeFormats, null, DateTimeStyles.None));
		}

		public override string ToString()
		{
			return _value.ToString("HH:mm:ss.fffffffzzz", CultureInfo.InvariantCulture);
		}
	}
}
