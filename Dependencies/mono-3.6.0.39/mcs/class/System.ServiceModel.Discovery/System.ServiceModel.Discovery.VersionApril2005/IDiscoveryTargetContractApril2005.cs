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

namespace System.ServiceModel.Discovery.VersionApril2005
{
	[ServiceContract (Name = "TargetService", Namespace = MessageContractsApril2005.NS, CallbackContract = typeof (IDiscoveryTargetCallbackContractApril2005))]
	internal interface IDiscoveryTargetContractApril2005
	{
		[OperationContract (Name = "ProbeApril2005", Action = MessageContractsApril2005.ProbeAction, AsyncPattern = true, IsOneWay = true)]
		IAsyncResult BeginFind (MessageContractsApril2005.FindRequest message, AsyncCallback callback, object state);

		void EndFind (IAsyncResult result);

		[OperationContract (Name = "ResolveApril2005", Action = MessageContractsApril2005.ResolveAction, AsyncPattern = true, IsOneWay = true)]
		IAsyncResult BeginResolve (MessageContractsApril2005.ResolveRequest message, AsyncCallback callback, object state);

		void EndResolve (IAsyncResult result);

		[OperationContract (Name = "HelloApril2005", Action = MessageContractsApril2005.HelloAction, IsOneWay = true, AsyncPattern = true)]
		IAsyncResult BeginOnlineAnnouncement (MessageContractsApril2005.OnlineAnnouncement message, AsyncCallback callback, object state);

		void EndOnlineAnnouncement (IAsyncResult result);
	}

	internal interface IDiscoveryTargetCallbackContractApril2005
	{
		[OperationContract (Name = "ProbeMatchesApril2005", Action = MessageContractsApril2005.ProbeMatchAction, IsOneWay = true)]
		void ReplyFind (MessageContractsApril2005.FindResponse message);

		[OperationContract (Name = "ResolveMatchesApril2005", Action = MessageContractsApril2005.ResolveMatchAction, IsOneWay = true)]
		void ReplyResolve (MessageContractsApril2005.ResolveResponse message);
	}
}
