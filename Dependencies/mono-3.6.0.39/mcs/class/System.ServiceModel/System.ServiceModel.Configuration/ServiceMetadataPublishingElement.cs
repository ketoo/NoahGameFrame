//
// ServiceMetadataPublishingElement.cs
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
	public sealed class ServiceMetadataPublishingElement
		 : BehaviorExtensionElement
	{

		public override Type BehaviorType {
			get { return typeof (ServiceMetadataBehavior); }
		}

		[ConfigurationProperty ("externalMetadataLocation",
			 Options = ConfigurationPropertyOptions.None)]
		public Uri ExternalMetadataLocation {
			get { return (Uri) base ["externalMetadataLocation"]; }
			set { base ["externalMetadataLocation"] = value; }
		}

		[StringValidator (MinLength = 0)]
		[ConfigurationProperty ("httpGetBinding", DefaultValue = "")]
		public string HttpGetBinding {
			get { return (string) base ["httpGetBinding"]; }
			set { base ["httpGetBinding"] = value; }
		}

		[ConfigurationProperty ("httpGetBindingConfiguration", DefaultValue = "")]
		[StringValidator (MinLength = 0)]
		public string HttpGetBindingConfiguration {
			get { return (string) base ["httpGetBindingConfiguration"]; }
			set { base ["httpGetBindingConfiguration"] = value; }
		}

		[ConfigurationProperty ("httpGetEnabled",
			 Options = ConfigurationPropertyOptions.None,
			DefaultValue = false)]
		public bool HttpGetEnabled {
			get { return (bool) base ["httpGetEnabled"]; }
			set { base ["httpGetEnabled"] = value; }
		}

		[ConfigurationProperty ("httpGetUrl",
			 Options = ConfigurationPropertyOptions.None)]
		public Uri HttpGetUrl {
			get { return (Uri) base ["httpGetUrl"]; }
			set { base ["httpGetUrl"] = value; }
		}

		[StringValidator (MinLength = 0)]
		[ConfigurationProperty ("httpsGetBinding", DefaultValue = "")]
		public string HttpsGetBinding {
			get { return (string) base ["httpsGetBinding"]; }
			set { base ["httpsGetBinding"] = value; }
		}

		[ConfigurationProperty ("httpsGetBindingConfiguration", DefaultValue = "")]
		[StringValidator (MinLength = 0)]
		public string HttpsGetBindingConfiguration {
			get { return (string) base ["httpsGetBindingConfiguration"]; }
			set { base ["httpsGetBindingConfiguration"] = value; }
		}

		[ConfigurationProperty ("httpsGetEnabled",
			 Options = ConfigurationPropertyOptions.None,
			DefaultValue = false)]
		public bool HttpsGetEnabled {
			get { return (bool) base ["httpsGetEnabled"]; }
			set { base ["httpsGetEnabled"] = value; }
		}

		[ConfigurationProperty ("httpsGetUrl",
			 Options = ConfigurationPropertyOptions.None)]
		public Uri HttpsGetUrl {
			get { return (Uri) base ["httpsGetUrl"]; }
			set { base ["httpsGetUrl"] = value; }
		}

		protected override ConfigurationPropertyCollection Properties {
			get { return base.Properties; }
		}

		protected internal override object CreateBehavior () {
			ServiceMetadataBehavior b = new ServiceMetadataBehavior ();
			b.HttpGetEnabled = HttpGetEnabled;
			b.HttpsGetEnabled = HttpsGetEnabled;
			b.HttpGetUrl = HttpGetUrl;
			b.HttpsGetUrl = HttpsGetUrl;
			if (!String.IsNullOrEmpty (HttpGetBinding))
				b.HttpGetBinding = ConfigUtil.CreateBinding (HttpGetBinding, HttpGetBindingConfiguration);
			if (!String.IsNullOrEmpty (HttpsGetBinding))
				b.HttpsGetBinding = ConfigUtil.CreateBinding (HttpsGetBinding, HttpsGetBindingConfiguration);
			return b;
		}

		[ConfigurationPropertyAttribute ("policyVersion",
			DefaultValue = "Default")]
		[TypeConverter (typeof (PolicyVersionConverter))]
		public PolicyVersion PolicyVersion {
			get { return (PolicyVersion) base ["policyVersion"]; }
			set { base ["policyVersion"] = value; }
		}

	}

}
