//
// DateTimeSerializationSection.cs
//
// Author:
//	Atsushi Enomoto  <atsushi@ximian.com>
//
// Copyright (C) 2006 Novell, Inc.
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

#if NET_2_0 && CONFIGURATION_DEP
using System;
using System.Collections;
using System.Configuration;
using System.Globalization;
using System.Xml;
using System.Text;

namespace System.Xml.Serialization.Configuration
{
	public sealed class DateTimeSerializationSection : ConfigurationSection
	{
		static ConfigurationPropertyCollection properties;
		static ConfigurationProperty mode;

		static DateTimeSerializationSection ()
		{
			properties = new ConfigurationPropertyCollection ();
			mode = new ConfigurationProperty ("mode", typeof (DateTimeSerializationMode), DateTimeSerializationMode.Roundtrip, ConfigurationPropertyOptions.None);
			properties.Add (mode);
		}

		public enum DateTimeSerializationMode
		{
			Default,
			Roundtrip,
			Local,
		}

		[ConfigurationProperty ("mode", DefaultValue = DateTimeSerializationMode.Roundtrip, Options = ConfigurationPropertyOptions.None)]
		public DateTimeSerializationMode Mode {
			get { return (DateTimeSerializationMode) this [mode]; }
			set { this [mode] = value; }
		}

		protected override ConfigurationPropertyCollection Properties {
			get { return properties; }
		}
	}
}

#endif
