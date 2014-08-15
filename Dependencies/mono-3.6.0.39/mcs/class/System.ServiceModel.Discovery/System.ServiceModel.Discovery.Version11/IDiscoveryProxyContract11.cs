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

namespace System.ServiceModel.Discovery.Version11
{
	[ServiceContract (Name = "DiscoveryProxy", Namespace = MessageContracts11.NS)]
	internal interface IDiscoveryProxyContract11
	{
		[OperationContract (Name = "Probe11", Action = MessageContracts11.ProbeAction, AsyncPattern = true, ReplyAction = MessageContracts11.ProbeMatchAction)]
		IAsyncResult BeginFind (MessageContracts11.FindRequest message, AsyncCallback callback, object state);

		MessageContracts11.FindResponse EndFind (IAsyncResult result);

		[OperationContract (Name = "Resolve11", Action = MessageContracts11.ResolveAction, AsyncPattern = true, ReplyAction = MessageContracts11.ResolveMatchAction)]
		IAsyncResult BeginResolve (MessageContracts11.ResolveRequest message, AsyncCallback callback, object state);

		MessageContracts11.ResolveResponse EndResolve (IAsyncResult result);
	}
}
