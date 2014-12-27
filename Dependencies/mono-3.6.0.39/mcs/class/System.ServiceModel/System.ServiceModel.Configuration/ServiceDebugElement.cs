//
// ServiceDebugElement.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2006 Novell, Inc.  http://www.novell.com
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
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Configuration;
using System.Net;
using System.Net.Security;
using System.Reflection;
using System.Security.Cryptography.X509Certificates;
using System.Security.Principal;
using System.IdentityModel.Claims;
using System.IdentityModel.Policy;
using System.IdentityModel.Tokens;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Description;
using System.ServiceModel.Diagnostics;
using System.ServiceModel.Dispatcher;
using System.ServiceModel.MsmqIntegration;
using System.ServiceModel.PeerResolvers;
using System.ServiceModel.Security;
using System.Runtime.Serialization;
using System.Text;
using System.Xml;

namespace System.ServiceModel.Configuration
{
	public sealed class ServiceDebugElement
		 : BehaviorExtensionElement
	{
		public ServiceDebugElement () {
		}


		// Properties

		public override Type BehaviorType {
			get { return typeof (ServiceDebugBehavior); }
		}

		[StringValidator (MinLength = 0)]
		[ConfigurationProperty ("httpHelpPageBinding", DefaultValue = "")]
		public string HttpHelpPageBinding {
			get { return (string) base ["httpHelpPageBinding"]; }
			set { base ["httpHelpPageBinding"] = value; }
		}

		[ConfigurationProperty ("httpHelpPageBindingConfiguration", DefaultValue = "")]
		[StringValidator (MinLength = 0)]
		public string HttpHelpPageBindingConfiguration {
			get { return (string) base ["httpHelpPageBindingConfiguration"]; }
			set { base ["httpHelpPageBindingConfiguration"] = value; }
		}

		[ConfigurationProperty ("httpHelpPageEnabled",
			 Options = ConfigurationPropertyOptions.None,
			DefaultValue = true)]
		public bool HttpHelpPageEnabled {
			get { return (bool) base ["httpHelpPageEnabled"]; }
			set { base ["httpHelpPageEnabled"] = value; }
		}

		[ConfigurationProperty ("httpHelpPageUrl",
			 Options = ConfigurationPropertyOptions.None)]
		public Uri HttpHelpPageUrl {
			get { return (Uri) base ["httpHelpPageUrl"]; }
			set { base ["httpHelpPageUrl"] = value; }
		}

		[StringValidator (MinLength = 0)]
		[ConfigurationProperty ("httpsHelpPageBinding", DefaultValue = "")]
		public string HttpsHelpPageBinding {
			get { return (string) base ["httpsHelpPageBinding"]; }
			set { base ["httpsHelpPageBinding"] = value; }
		}

		[ConfigurationProperty ("httpsHelpPageBindingConfiguration", DefaultValue = "")]
		[StringValidator (MinLength = 0)]
		public string HttpsHelpPageBindingConfiguration {
			get { return (string) base ["httpsHelpPageBindingConfiguration"]; }
			set { base ["httpsHelpPageBindingConfiguration"] = value; }
		}

		[ConfigurationProperty ("httpsHelpPageEnabled",
			 Options = ConfigurationPropertyOptions.None,
			DefaultValue = true)]
		public bool HttpsHelpPageEnabled {
			get { return (bool) base ["httpsHelpPageEnabled"]; }
			set { base ["httpsHelpPageEnabled"] = value; }
		}

		[ConfigurationProperty ("httpsHelpPageUrl",
			 Options = ConfigurationPropertyOptions.None)]
		public Uri HttpsHelpPageUrl {
			get { return (Uri) base ["httpsHelpPageUrl"]; }
			set { base ["httpsHelpPageUrl"] = value; }
		}

		[ConfigurationProperty ("includeExceptionDetailInFaults",
			 Options = ConfigurationPropertyOptions.None,
			DefaultValue = false)]
		public bool IncludeExceptionDetailInFaults {
			get { return (bool) base ["includeExceptionDetailInFaults"]; }
			set { base ["includeExceptionDetailInFaults"] = value; }
		}

		protected override ConfigurationPropertyCollection Properties {
			get { return base.Properties; }
		}

		protected internal override object CreateBehavior ()
		{
			var ret = new ServiceDebugBehavior () {
				HttpHelpPageEnabled = HttpHelpPageEnabled,
				HttpsHelpPageEnabled = HttpsHelpPageEnabled,
				HttpHelpPageUrl = HttpHelpPageUrl,
				HttpsHelpPageUrl = HttpsHelpPageUrl,
				IncludeExceptionDetailInFaults = IncludeExceptionDetailInFaults,
			};
			if (!String.IsNullOrEmpty (HttpHelpPageBinding))
				ret.HttpHelpPageBinding = ConfigUtil.CreateBinding (HttpHelpPageBinding, HttpHelpPageBindingConfiguration);
			if (!String.IsNullOrEmpty (HttpsHelpPageBinding))
				ret.HttpsHelpPageBinding = ConfigUtil.CreateBinding (HttpsHelpPageBinding, HttpsHelpPageBindingConfiguration);
			return ret;
		}

	}

}
