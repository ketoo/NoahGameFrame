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
	[MonoTODO]
	public abstract class DiscoveryProxy : IDiscoveryProxyContract11, IDiscoveryProxyContractApril2005, IDiscoveryProxyContractCD1, IDiscoveryTargetContract11, IDiscoveryTargetContractApril2005, IDiscoveryTargetContractCD1
	{
		protected DiscoveryProxy ()
			: this (new DiscoveryMessageSequenceGenerator ())
		{
		}

		protected DiscoveryProxy (DiscoveryMessageSequenceGenerator messageSequenceGenerator)
			: this (messageSequenceGenerator, 0)
		{
		}

		protected DiscoveryProxy (DiscoveryMessageSequenceGenerator messageSequenceGenerator, int duplicateMessageHistoryLength)
		{
		}

		internal DiscoveryMessageSequenceGenerator DiscoveryMessageSequenceGenerator { get; private set; }

		internal int DuplicateMessageHistoryLength { get; private set; }

		protected virtual IAsyncResult BeginShouldRedirectFind (FindCriteria resolveCriteria, AsyncCallback callback, Object state)
		{
			throw new NotImplementedException ();
		}

		protected virtual IAsyncResult BeginShouldRedirectResolve (ResolveCriteria findCriteria, AsyncCallback callback, Object state)
		{
			throw new NotImplementedException ();
		}

		protected virtual bool EndShouldRedirectFind (IAsyncResult result, out Collection<EndpointDiscoveryMetadata> redirectionEndpoints)
		{
			throw new NotImplementedException ();
		}

		protected virtual bool EndShouldRedirectResolve (IAsyncResult result, out Collection<EndpointDiscoveryMetadata> redirectionEndpoints)
		{
			throw new NotImplementedException ();
		}

		protected abstract IAsyncResult OnBeginFind (FindRequestContext findRequestContext, AsyncCallback callback, Object state);
		protected abstract IAsyncResult OnBeginOfflineAnnouncement (DiscoveryMessageSequence messageSequence, EndpointDiscoveryMetadata endpointDiscoveryMetadata, AsyncCallback callback, Object state);
		protected abstract IAsyncResult OnBeginOnlineAnnouncement (DiscoveryMessageSequence messageSequence, EndpointDiscoveryMetadata endpointDiscoveryMetadata, AsyncCallback callback, Object state);
		protected abstract IAsyncResult OnBeginResolve (ResolveCriteria resolveCriteria, AsyncCallback callback, Object state);
		protected abstract void OnEndFind (IAsyncResult result);
		protected abstract void OnEndOfflineAnnouncement (IAsyncResult result);
		protected abstract void OnEndOnlineAnnouncement (IAsyncResult result);
		protected abstract EndpointDiscoveryMetadata OnEndResolve (IAsyncResult result);


		#region service contract implementation
		
		// IDiscoveryProxyContract11
		IAsyncResult IDiscoveryProxyContract11.BeginFind (MessageContracts11.FindRequest message, AsyncCallback callback, object state)
		{
			return OnBeginFind (new DefaultFindRequestContext (message.Body.ToFindCriteria ()), callback, state);
		}

		MessageContracts11.FindResponse IDiscoveryProxyContract11.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
			throw new NotImplementedException ();
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
			return OnBeginFind (new DefaultFindRequestContext (message.Body.ToFindCriteria ()), callback, state);
		}

		MessageContractsApril2005.FindResponse IDiscoveryProxyContractApril2005.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
			throw new NotImplementedException ();
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
			return OnBeginFind (new DefaultFindRequestContext (message.Body.ToFindCriteria ()), callback, state);
		}

		MessageContractsCD1.FindResponse IDiscoveryProxyContractCD1.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
			throw new NotImplementedException ();
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
			return OnBeginFind (new DefaultFindRequestContext (message.Body.ToFindCriteria ()), callback, state);
		}

		void IDiscoveryTargetContract11.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
		}

		IAsyncResult IDiscoveryTargetContract11.BeginResolve (MessageContracts11.ResolveRequest message, AsyncCallback callback, object state)
		{
			return OnBeginResolve (message.Body.ToResolveCriteria (), callback, state);
		}

		void IDiscoveryTargetContract11.EndResolve (IAsyncResult result)
		{
			OnEndResolve (result);
		}

		IAsyncResult IDiscoveryTargetContract11.BeginOnlineAnnouncement (MessageContracts11.OnlineAnnouncement msg, AsyncCallback callback, object state)
		{
			var edm = msg.EndpointDiscoveryMetadata != null ? msg.EndpointDiscoveryMetadata.ToEndpointDiscoveryMetadata () : null;
			return OnBeginOnlineAnnouncement (DiscoveryMessageSequenceGenerator.Next (), edm, callback, state);
		}

		void IDiscoveryTargetContract11.EndOnlineAnnouncement (IAsyncResult result)
		{
			OnEndOnlineAnnouncement (result);
		}

		// IDiscoveryTargetContractApril2005
		IAsyncResult IDiscoveryTargetContractApril2005.BeginFind (MessageContractsApril2005.FindRequest message, AsyncCallback callback, object state)
		{
			return OnBeginFind (new DefaultFindRequestContext (message.Body.ToFindCriteria ()), callback, state);
		}

		void IDiscoveryTargetContractApril2005.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
		}

		IAsyncResult IDiscoveryTargetContractApril2005.BeginResolve (MessageContractsApril2005.ResolveRequest message, AsyncCallback callback, object state)
		{
			return OnBeginResolve (message.Body.ToResolveCriteria (), callback, state);
		}

		void IDiscoveryTargetContractApril2005.EndResolve (IAsyncResult result)
		{
			OnEndResolve (result);
		}

		IAsyncResult IDiscoveryTargetContractApril2005.BeginOnlineAnnouncement (MessageContractsApril2005.OnlineAnnouncement msg, AsyncCallback callback, object state)
		{
			var edm = msg.EndpointDiscoveryMetadata != null ? msg.EndpointDiscoveryMetadata.ToEndpointDiscoveryMetadata () : null;
			return OnBeginOnlineAnnouncement (DiscoveryMessageSequenceGenerator.Next (), edm, callback, state);
		}

		void IDiscoveryTargetContractApril2005.EndOnlineAnnouncement (IAsyncResult result)
		{
			OnEndOnlineAnnouncement (result);
		}

		// IDiscoveryTargetContractCD1
		IAsyncResult IDiscoveryTargetContractCD1.BeginFind (MessageContractsCD1.FindRequest message, AsyncCallback callback, object state)
		{
			return OnBeginFind (new DefaultFindRequestContext (message.Body.ToFindCriteria ()), callback, state);
		}

		void IDiscoveryTargetContractCD1.EndFind (IAsyncResult result)
		{
			OnEndFind (result);
		}

		IAsyncResult IDiscoveryTargetContractCD1.BeginResolve (MessageContractsCD1.ResolveRequest message, AsyncCallback callback, object state)
		{
			return OnBeginResolve (message.Body.ToResolveCriteria (), callback, state);
		}

		void IDiscoveryTargetContractCD1.EndResolve (IAsyncResult result)
		{
			OnEndResolve (result);
		}

		IAsyncResult IDiscoveryTargetContractCD1.BeginOnlineAnnouncement (MessageContractsCD1.OnlineAnnouncement msg, AsyncCallback callback, object state)
		{
			var edm = msg.EndpointDiscoveryMetadata != null ? msg.EndpointDiscoveryMetadata.ToEndpointDiscoveryMetadata () : null;
			return OnBeginOnlineAnnouncement (DiscoveryMessageSequenceGenerator.Next (), edm, callback, state);
		}

		void IDiscoveryTargetContractCD1.EndOnlineAnnouncement (IAsyncResult result)
		{
			OnEndOnlineAnnouncement (result);
		}

		#endregion
	}
}
