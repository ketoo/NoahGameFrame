// 
// System.Web.Services.Description.SoapBodyBinding.cs
//
// Author:
//   Tim Coleman (tim@timcoleman.com)
//
// Copyright (C) Tim Coleman, 2002
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

using System.ComponentModel;
using System.Web.Services.Configuration;
using System.Xml.Serialization;

namespace System.Web.Services.Description {
	[XmlFormatExtension ("body", "http://schemas.xmlsoap.org/wsdl/soap/", typeof (InputBinding), typeof (OutputBinding), typeof (MimePart))]
	public class SoapBodyBinding : ServiceDescriptionFormatExtension {

		#region Fields
		
		string encoding;
		string ns;
		string[] parts;
		string partsString;
		SoapBindingUse use;

		#endregion // Fields

		#region Constructors
		
		public SoapBodyBinding ()
		{
			encoding = String.Empty;
			ns = String.Empty;
			parts = null;
			partsString = null;
			use = SoapBindingUse.Default;
		}
		
		#endregion // Constructors

		#region Properties

		[DefaultValue ("")]
		[XmlAttribute ("encodingStyle")]
		public string Encoding {
			get { return encoding; }
			set { encoding = value; }
		}

		[DefaultValue ("")]
		[XmlAttribute ("namespace")]
		public string Namespace {
			get { return ns; }
			set { ns = value; }
		}

		[XmlIgnore]
		public string[] Parts {
			get { return parts; }
			set {
				parts = value;
				if (value == null)
					partsString = null;
				else
					partsString = String.Join(" ", value);
			}
		}

		[XmlAttribute ("parts")]
		public string PartsString {
			get { return partsString; }
			set {
				partsString = value;
				if (value == null)
					parts = null;
				else
					parts = value.Split(' ');
			}
		}

		[DefaultValue (SoapBindingUse.Default)]
		[XmlAttribute ("use")]
		public SoapBindingUse Use {
			get { return use; }
			set { use = value; }
		}

		#endregion // Properties
	}
}
