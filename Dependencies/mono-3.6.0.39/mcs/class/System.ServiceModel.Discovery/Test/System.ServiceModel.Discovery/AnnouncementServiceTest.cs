//
// Author: Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2010 Novell, Inc (http://www.novell.com)
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
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Description;
using System.ServiceModel.Discovery;
using System.ServiceModel.Dispatcher;
using NUnit.Framework;

namespace MonoTests.System.ServiceModel.Discovery
{
	[TestFixture]
	public class AnnouncementServiceTest
	{
		[Test]
		public void ContractInterface ()
		{
			// This will raise IOE if AnnouncementService does not implement any of the contract interfaces.
			var host = new ServiceHost (typeof (AnnouncementService));
			host.AddServiceEndpoint (new UdpAnnouncementEndpoint (DiscoveryVersion.WSDiscovery11));
			host.AddServiceEndpoint (new UdpAnnouncementEndpoint (DiscoveryVersion.WSDiscoveryApril2005, "soap.udp://239.255.255.250:3703/"));
			host.AddServiceEndpoint (new UdpAnnouncementEndpoint (DiscoveryVersion.WSDiscoveryCD1, "soap.udp://239.255.255.250:3704/"));

			var v11 = host.Description.Endpoints.FirstOrDefault (ep => ep.Contract.Namespace == "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01");
			Assert.IsNotNull (v11, "v11");
			Assert.AreEqual ("CustomBinding_Client", v11.Name, "v11.Name");

			var v200504 = host.Description.Endpoints.FirstOrDefault (ep => ep.Contract.Namespace == "http://schemas.xmlsoap.org/ws/2005/04/discovery");
			Assert.IsNotNull (v200504, "v200504");
			Assert.AreEqual ("CustomBinding_Client", v200504.Name, "v200504.Name");

			var vCD1 = host.Description.Endpoints.FirstOrDefault (ep => ep.Contract.Namespace == "http://docs.oasis-open.org/ws-dd/ns/discovery/2008/09");
			Assert.IsNotNull (vCD1, "vCD1");
			Assert.AreEqual ("CustomBinding_Client", vCD1.Name, "v11.Name");
		}
	}
}
