//
// ServiceElement.cs
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
	public sealed class ServiceElement
		 : ConfigurationElement
	{
		// Static Fields
		static ConfigurationPropertyCollection properties;
		static ConfigurationProperty behavior_configuration;
		static ConfigurationProperty endpoints;
		static ConfigurationProperty host;
		static ConfigurationProperty name;

		static ServiceElement ()
		{
			properties = new ConfigurationPropertyCollection ();
			behavior_configuration = new ConfigurationProperty ("behaviorConfiguration",
				typeof (string), "", new StringConverter (), new StringValidator (0, int.MaxValue, null),
				ConfigurationPropertyOptions.None);

			endpoints = new ConfigurationProperty ("",
				typeof (ServiceEndpointElementCollection), null, null, null,
				ConfigurationPropertyOptions.IsDefaultCollection);

			host = new ConfigurationProperty ("host",
				typeof (HostElement), null, null, null,
				ConfigurationPropertyOptions.None);

			name = new ConfigurationProperty ("name",
				typeof (string), null, new StringConverter (), new StringValidator (1, int.MaxValue, null),
				ConfigurationPropertyOptions.IsRequired| ConfigurationPropertyOptions.IsKey);

			properties.Add (behavior_configuration);
			properties.Add (endpoints);
			properties.Add (host);
			properties.Add (name);
		}

		public ServiceElement ()
		{
		}


		// Properties

		[StringValidator ( MinLength = 0,
			MaxLength = int.MaxValue,
			 InvalidCharacters = null)]
		[ConfigurationProperty ("behaviorConfiguration",
			 Options = ConfigurationPropertyOptions.None,
			 DefaultValue = "")]
		public string BehaviorConfiguration {
			get { return (string) base [behavior_configuration]; }
			set { base [behavior_configuration] = value; }
		}

		[ConfigurationProperty ("",
			 Options = ConfigurationPropertyOptions.IsDefaultCollection,
			IsDefaultCollection = true)]
		public ServiceEndpointElementCollection Endpoints {
			get { return (ServiceEndpointElementCollection) base [endpoints]; }
		}

		[ConfigurationProperty ("host",
			 Options = ConfigurationPropertyOptions.None)]
		public HostElement Host {
			get { return (HostElement) base [host]; }
		}

		[ConfigurationProperty ("name",
			 Options = ConfigurationPropertyOptions.IsRequired | ConfigurationPropertyOptions.IsKey,
			IsRequired = true,
			IsKey = true)]
		[StringValidator ( MinLength = 1,
			MaxLength = int.MaxValue,
			 InvalidCharacters = null)]
		public string Name {
			get { return (string) base [name]; }
			set { base [name] = value; }
		}

		protected override ConfigurationPropertyCollection Properties {
			get { return properties; }
		}


	}

}
