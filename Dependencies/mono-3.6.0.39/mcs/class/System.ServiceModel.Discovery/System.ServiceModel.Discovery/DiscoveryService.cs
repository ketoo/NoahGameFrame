//
// Author: Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2009,2010 Novell, Inc (http://www.novell.com)
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
using System.ServiceModel.Discovery.Version11;
using System.ServiceModel.Discovery.VersionApril2005;
using System.ServiceModel.Discovery.VersionCD1;

namespace System.ServiceModel.Discovery
{
	public abstract class DiscoveryService : IDiscoveryProxyContract11, IDiscoveryProxyContractApril2005, IDiscoveryProxyContractCD1, IDiscoveryTargetContract11, IDiscoveryTargetContractApril2005, IDiscoveryTargetContractCD1
	{
		protected DiscoveryService ()
			: this (new DiscoveryMessageSequenceGenerator ())
		{
		}

		protected DiscoveryService (DiscoveryMessageSequenceGenerator discoveryMessageSequenceGenerator)
			: this (discoveryMessageSequenceGenerator, 0)
		{
		}

		protected DiscoveryService (DiscoveryMessageSequenceGenerator discoveryMessageSequenceGenerator, int duplicateMessageHistoryLength)
		{
			DiscoveryMessageSequenceGenerator = discoveryMessageSequenceGenerator;
			DuplicateMessageHistoryLength = duplicateMessageHistoryLength;
		}

		internal DiscoveryMessageSequenceGenerator DiscoveryMessageSequenceGenerator { get; private set; }

		internal int DuplicateMessageHistoryLength { get; private set; }

		protected abstract IAsyncResult OnBeginFind (FindRequestContext findRequestContext, AsyncCallback callback, Object state);

		protected abstract IAsyncResult OnBeginResolve (ResolveCriteria resolveCriteria, AsyncCallback callback, Object state);

		protected abstract void OnEndFind (IAsyncResult result);

		protected abstract EndpointDiscoveryMetadata OnEndResolve (IAsyncResult result);

		#region service contract implementation
		
		// IDiscoveryProxyContract11
		
		FindRequestContext find_context;
		
		IAsyncResult IDiscoveryProxyContract11.BeginFind (MessageContracts11.FindRequest message, AsyncCallback callback, object state)
		{
			if (find_context != null)
				throw new InvalidOperationException ("Another async Find operation is ongoing");
			find_context = new DefaultFindRequestContext (message.Body.ToFindCriteria ());
			return OnBeginFind (find_context, callback, state);
		}

		MessageContracts11.FindResponse IDiscoveryProxyContract11.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
			return CreateFindResponse11 ();
		}

		MessageContracts11.FindResponse CreateFindResponse11 ()
		{
			var l = new MessageContracts11.FindResponse11 ();
			foreach (var edm in find_context.Endpoints)
				l.Add (new EndpointDiscoveryMetadata11 (edm));
			find_context = null;
			return new MessageContracts11.FindResponse () { Body = l };
		}

		IAsyncResult IDiscoveryProxyContract11.BeginResolve (MessageContracts11.ResolveRequest message, AsyncCallback callback, object state)
		{
			return OnBeginResolve (message.Body.ToResolveCriteria (), callback, state);
		}

		MessageContracts11.ResolveResponse IDiscoveryProxyContract11.EndResolve (IAsyncResult result)
		{
			var ret = OnEndResolve (result);
			return new MessageContracts11.ResolveResponse () { MessageSequence = new DiscoveryMessageSequence11 (DiscoveryMessageSequenceGenerator.Next ()), Body = new EndpointDiscoveryMetadata11 (ret) };
		}
		
		// IDiscoveryProxyContractApril2005
		IAsyncResult IDiscoveryProxyContractApril2005.BeginFind (MessageContractsApril2005.FindRequest message, AsyncCallback callback, object state)
		{
			if (find_context != null)
				throw new InvalidOperationException ("Another async Find operation is ongoing");
			find_context = new DefaultFindRequestContext (message.Body.ToFindCriteria ());
			return OnBeginFind (find_context, callback, state);
		}

		MessageContractsApril2005.FindResponse IDiscoveryProxyContractApril2005.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
			return CreateFindResponseApril2005 ();
		}

		MessageContractsApril2005.FindResponse CreateFindResponseApril2005 ()
		{
			var l = new MessageContractsApril2005.FindResponseApril2005 ();
			foreach (var edm in find_context.Endpoints)
				l.Add (new EndpointDiscoveryMetadataApril2005 (edm));
			find_context = null;
			return new MessageContractsApril2005.FindResponse () { Body = l };
		}

		IAsyncResult IDiscoveryProxyContractApril2005.BeginResolve (MessageContractsApril2005.ResolveRequest message, AsyncCallback callback, object state)
		{
			return OnBeginResolve (message.Body.ToResolveCriteria (), callback, state);
		}

		MessageContractsApril2005.ResolveResponse IDiscoveryProxyContractApril2005.EndResolve (IAsyncResult result)
		{
			var ret = OnEndResolve (result);
			return new MessageContractsApril2005.ResolveResponse () { MessageSequence = new DiscoveryMessageSequenceApril2005 (DiscoveryMessageSequenceGenerator.Next ()), Body = new EndpointDiscoveryMetadataApril2005 (ret) };
		}
		
		// IDiscoveryProxyContractCD1
		IAsyncResult IDiscoveryProxyContractCD1.BeginFind (MessageContractsCD1.FindRequest message, AsyncCallback callback, object state)
		{
			if (find_context != null)
				throw new InvalidOperationException ("Another async Find operation is ongoing");
			find_context = new DefaultFindRequestContext (message.Body.ToFindCriteria ());
			return OnBeginFind (find_context, callback, state);
		}

		MessageContractsCD1.FindResponse IDiscoveryProxyContractCD1.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
			return CreateFindResponseCD1 ();
		}

		MessageContractsCD1.FindResponse CreateFindResponseCD1 ()
		{
			var l = new MessageContractsCD1.FindResponseCD1 ();
			foreach (var edm in find_context.Endpoints)
				l.Add (new EndpointDiscoveryMetadataCD1 (edm));
			find_context = null;
			return new MessageContractsCD1.FindResponse () { Body = l };
		}

		IAsyncResult IDiscoveryProxyContractCD1.BeginResolve (MessageContractsCD1.ResolveRequest message, AsyncCallback callback, object state)
		{
			return OnBeginResolve (message.Body.ToResolveCriteria (), callback, state);
		}

		MessageContractsCD1.ResolveResponse IDiscoveryProxyContractCD1.EndResolve (IAsyncResult result)
		{
			var ret = OnEndResolve (result);
			return new MessageContractsCD1.ResolveResponse () { MessageSequence = new DiscoveryMessageSequenceCD1 (DiscoveryMessageSequenceGenerator.Next ()), Body = new EndpointDiscoveryMetadataCD1 (ret) };
		}

		// IDiscoveryTargetContract11
		IAsyncResult IDiscoveryTargetContract11.BeginFind (MessageContracts11.FindRequest message, AsyncCallback callback, object state)
		{
			find_context = new DefaultFindRequestContext (message.Body.ToFindCriteria ());
			return OnBeginFind (new DefaultFindRequestContext (message.Body.ToFindCriteria ()), callback, state);
		}

		void IDiscoveryTargetContract11.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
			var cb = OperationContext.Current.GetCallbackChannel<IDiscoveryTargetCallbackContract11> ();
			cb.ReplyFind (CreateFindResponse11 ());
		}

		IAsyncResult IDiscoveryTargetContract11.BeginResolve (MessageContracts11.ResolveRequest message, AsyncCallback callback, object state)
		{
			return OnBeginResolve (message.Body.ToResolveCriteria (), callback, state);
		}

		void IDiscoveryTargetContract11.EndResolve (IAsyncResult result)
		{
			OnEndResolve (result);
		}

		IAsyncResult IDiscoveryTargetContract11.BeginOnlineAnnouncement (MessageContracts11.OnlineAnnouncement message, AsyncCallback callback, object state)
		{
			// is it expected to be invoked??
			throw new NotImplementedException ();
		}

		void IDiscoveryTargetContract11.EndOnlineAnnouncement (IAsyncResult result)
		{
			// is it expected to be invoked??
			throw new NotImplementedException ();
		}

		// IDiscoveryTargetContractApril2005
		IAsyncResult IDiscoveryTargetContractApril2005.BeginFind (MessageContractsApril2005.FindRequest message, AsyncCallback callback, object state)
		{
			find_context = new DefaultFindRequestContext (message.Body.ToFindCriteria ());
			return OnBeginFind (new DefaultFindRequestContext (message.Body.ToFindCriteria ()), callback, state);
		}

		void IDiscoveryTargetContractApril2005.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
			var cb = OperationContext.Current.GetCallbackChannel<IDiscoveryTargetCallbackContractApril2005> ();
			cb.ReplyFind (CreateFindResponseApril2005 ());
		}

		IAsyncResult IDiscoveryTargetContractApril2005.BeginResolve (MessageContractsApril2005.ResolveRequest message, AsyncCallback callback, object state)
		{
			return OnBeginResolve (message.Body.ToResolveCriteria (), callback, state);
		}

		void IDiscoveryTargetContractApril2005.EndResolve (IAsyncResult result)
		{
			OnEndResolve (result);
		}

		IAsyncResult IDiscoveryTargetContractApril2005.BeginOnlineAnnouncement (MessageContractsApril2005.OnlineAnnouncement message, AsyncCallback callback, object state)
		{
			// is it expected to be invoked??
			throw new NotImplementedException ();
		}

		void IDiscoveryTargetContractApril2005.EndOnlineAnnouncement (IAsyncResult result)
		{
			// is it expected to be invoked??
			throw new NotImplementedException ();
		}

		// IDiscoveryTargetContractCD1
		IAsyncResult IDiscoveryTargetContractCD1.BeginFind (MessageContractsCD1.FindRequest message, AsyncCallback callback, object state)
		{
			find_context = new DefaultFindRequestContext (message.Body.ToFindCriteria ());
			return OnBeginFind (new DefaultFindRequestContext (message.Body.ToFindCriteria ()), callback, state);
		}

		void IDiscoveryTargetContractCD1.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
			var cb = OperationContext.Current.GetCallbackChannel<IDiscoveryTargetCallbackContractCD1> ();
			cb.ReplyFind (CreateFindResponseCD1 ());
		}

		IAsyncResult IDiscoveryTargetContractCD1.BeginResolve (MessageContractsCD1.ResolveRequest message, AsyncCallback callback, object state)
		{
			return OnBeginResolve (message.Body.ToResolveCriteria (), callback, state);
		}

		void IDiscoveryTargetContractCD1.EndResolve (IAsyncResult result)
		{
			OnEndResolve (result);
		}

		IAsyncResult IDiscoveryTargetContractCD1.BeginOnlineAnnouncement (MessageContractsCD1.OnlineAnnouncement message, AsyncCallback callback, object state)
		{
			// is it expected to be invoked??
			throw new NotImplementedException ();
		}

		void IDiscoveryTargetContractCD1.EndOnlineAnnouncement (IAsyncResult result)
		{
			// is it expected to be invoked??
			throw new NotImplementedException ();
		}

		#endregion
	}

	internal class DefaultDiscoveryService : DiscoveryService
	{
		Action<FindRequestContext> find_delegate;
		Func<ResolveCriteria,EndpointDiscoveryMetadata> resolve_delegate;

		protected override IAsyncResult OnBeginFind (FindRequestContext findRequestContext, AsyncCallback callback, object state)
		{
			// FIXME: this is a workaround for (similar to) bug #633945.
			switch (Environment.OSVersion.Platform) {
			case PlatformID.Unix:
			case PlatformID.MacOSX:
				if (find_delegate == null)
					find_delegate = new Action<FindRequestContext> (Find);
				return find_delegate.BeginInvoke (findRequestContext, callback, state);
			default:
				Find (findRequestContext);
				var result = new TempAsyncResult (null, state);
				if (callback != null)
					callback (result);
				return result;
			}
		}

		protected override void OnEndFind (IAsyncResult result)
		{
			// FIXME: this is a workaround for (similar to) bug #633945.
			switch (Environment.OSVersion.Platform) {
			case PlatformID.Unix:
			case PlatformID.MacOSX:
				find_delegate.EndInvoke (result);
				break;
			default:
				break;
			}

			var oc = OperationContext.Current;
			var rmp = oc.IncomingMessageProperties [RemoteEndpointMessageProperty.Name] as RemoteEndpointMessageProperty;
			if (rmp != null)
				// FIXME: use appropriate port. Client does not listen at the sending port.
				oc.OutgoingMessageProperties.Add (RemoteEndpointMessageProperty.Name, new RemoteEndpointMessageProperty (rmp.Address, rmp.Port));
		}

		protected override IAsyncResult OnBeginResolve (ResolveCriteria resolveCriteria, AsyncCallback callback, object state)
		{
			// FIXME: this is a workaround for (similar to) bug #633945.
			switch (Environment.OSVersion.Platform) {
			case PlatformID.Unix:
			case PlatformID.MacOSX:
				if (resolve_delegate == null)
					resolve_delegate = new Func<ResolveCriteria,EndpointDiscoveryMetadata> (Resolve);
				return resolve_delegate.BeginInvoke (resolveCriteria, callback, state);
			default:
				var ret = Resolve (resolveCriteria);
				var result = new TempAsyncResult (ret, state);
				if (callback != null)
					callback (result);
				return result;
			}
		}

		protected override EndpointDiscoveryMetadata OnEndResolve (IAsyncResult result)
		{
			// FIXME: this is a workaround for (similar to) bug #633945.
			switch (Environment.OSVersion.Platform) {
			case PlatformID.Unix:
			case PlatformID.MacOSX:
				return resolve_delegate.EndInvoke (result);
			default:
				return (EndpointDiscoveryMetadata) ((TempAsyncResult) result).ReturnValue;
			}
		}

		void Find (FindRequestContext context)
		{
			throw new NotImplementedException ();
		}

		EndpointDiscoveryMetadata Resolve (ResolveCriteria criteria)
		{
			throw new NotImplementedException ();
		}
	}
}
