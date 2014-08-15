//
// ServiceThrottlingBehavior.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2005 Novell, Inc.  http://www.novell.com
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
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Dispatcher;

namespace System.ServiceModel.Description
{
	public class ServiceThrottlingBehavior : IServiceBehavior
	{
		public ServiceThrottlingBehavior ()
		{
		}

		int max_calls = 16, max_conn = 10, max_instance = 26;

		public int MaxConcurrentCalls {
			get { return max_calls; }
			set {
				if (value <= 0)
					throw new InvalidOperationException ("must be positive value");
				max_calls = value;
			}
		}

		public int MaxConcurrentSessions {
			get { return max_conn; }
			set {
				if (value <= 0)
					throw new InvalidOperationException ("must be positive value");
				max_conn = value;
			}
		}

		public int MaxConcurrentInstances {
			get { return max_instance; }
			set {
				if (value <= 0)
					throw new InvalidOperationException ("must be positive value");
				max_instance = value;
			}
		}

		void IServiceBehavior.AddBindingParameters (
			ServiceDescription description,
			ServiceHostBase serviceHostBase,
			Collection<ServiceEndpoint> endpoints,
			BindingParameterCollection parameters)
		{
		}

		void IServiceBehavior.ApplyDispatchBehavior (
			ServiceDescription description,
			ServiceHostBase serviceHostBase)
		{
			foreach (var cdb in serviceHostBase.ChannelDispatchers) {
				var cd = cdb as ChannelDispatcher;
				if (cd != null)
					cd.ServiceThrottle = new ServiceThrottle (cd) {
						MaxConcurrentCalls = this.MaxConcurrentCalls,
						MaxConcurrentSessions = this.MaxConcurrentSessions,
						MaxConcurrentInstances = this.MaxConcurrentInstances };
			}
		}

		void IServiceBehavior.Validate (
			ServiceDescription description,
			ServiceHostBase serviceHostBase)
		{
		}
	}
}
