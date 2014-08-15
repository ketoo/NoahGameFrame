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
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Description;
using System.ServiceModel.Dispatcher;
using System.ServiceModel.Discovery;

namespace System.ServiceModel.Discovery.VersionCD1
{
	[ServiceContract (Name = "DiscoveryProxy", Namespace = MessageContractsCD1.NS)]
	internal interface IDiscoveryProxyContractCD1
	{
		[OperationContract (Name = "ProbeCD1", Action = MessageContractsCD1.ProbeAction, AsyncPattern = true, ReplyAction = MessageContractsCD1.ProbeMatchAction)]
		IAsyncResult BeginFind (MessageContractsCD1.FindRequest message, AsyncCallback callback, object state);

		MessageContractsCD1.FindResponse EndFind (IAsyncResult result);

		[OperationContract (Name = "ResolveCD1", Action = MessageContractsCD1.ResolveAction, AsyncPattern = true, ReplyAction = MessageContractsCD1.ResolveMatchAction)]
		IAsyncResult BeginResolve (MessageContractsCD1.ResolveRequest message, AsyncCallback callback, object state);

		MessageContractsCD1.ResolveResponse EndResolve (IAsyncResult result);
	}
}
