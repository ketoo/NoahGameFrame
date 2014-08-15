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
using System.Net.Sockets;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Description;
using System.ServiceModel.Discovery;
using System.ServiceModel.Dispatcher;
using NUnit.Framework;

namespace MonoTests.System.ServiceModel.Discovery
{
	[TestFixture]
	public class UdpAnnouncementEndpointTest
	{
		[Test]
		public void DefaultValues ()
		{
			var de = new UdpAnnouncementEndpoint ();
			Assert.AreEqual (DiscoveryVersion.WSDiscovery11, de.DiscoveryVersion, "#1");
			Assert.AreEqual (TimeSpan.FromMilliseconds (500), de.MaxAnnouncementDelay, "#2");
			Assert.IsNotNull (de.Contract, "#11"); // some version-dependent internal type.
			Assert.AreEqual ("http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01", de.Contract.Namespace, "#11-2");
			Assert.AreEqual ("Client", de.Contract.Name, "#11-3");
			Assert.IsNotNull (de.Binding, "#12");
			TransportBindingElement tbe = null;
			Assert.IsTrue (de.Binding.CreateBindingElements ().Any (be => (tbe = be as TransportBindingElement) != null && tbe.Scheme == "soap.udp"), "#12-2");
			Assert.IsNotNull (de.Address, "#13");
			Assert.AreEqual (DiscoveryVersion.WSDiscovery11.AdhocAddress, de.Address.Uri, "#13-2");
			Assert.AreEqual (Socket.SupportsIPv4 ? UdpAnnouncementEndpoint.DefaultIPv4MulticastAddress : UdpAnnouncementEndpoint.DefaultIPv6MulticastAddress, de.ListenUri, "#14");

			// .NET has two endpoint behaviors: DiscoveryViaBehavior and UdpContractFilterBehavior. Not worthy of testing that though.
			// Assert.AreEqual (2, de.Behaviors.Count, "#15");
			// foreach (var eb in de.Behaviors) Console.Error.WriteLine (eb);
		}
	}
}
