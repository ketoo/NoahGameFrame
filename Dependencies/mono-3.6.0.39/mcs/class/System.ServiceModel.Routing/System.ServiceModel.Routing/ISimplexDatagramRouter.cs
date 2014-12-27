using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Description;
using System.ServiceModel.Dispatcher;

namespace System.ServiceModel.Routing
{
	[ServiceContract (Namespace = "http://schemas.microsoft.com/netfx/2009/05/routing", SessionMode = SessionMode.Allowed)]
	public interface ISimplexDatagramRouter
	{
		[OperationContract (AsyncPattern = true, IsOneWay = true, Action = "*")]
		IAsyncResult BeginProcessMessage (Message message, AsyncCallback callback, object state);

		void EndProcessMessage (IAsyncResult result);
	}
}
