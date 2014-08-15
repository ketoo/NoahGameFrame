﻿using System;
using System.Collections.Generic;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Security;
using System.ServiceModel.Security.Tokens;
using System.Text;
using System.ServiceModel.MonoInternal;

namespace System.ServiceModel.Dispatcher
{
	// Its lifetime is per-call.
	// ServiceRuntimeChannel's lifetime is per-session.
	internal class InputOrReplyRequestProcessor : BaseRequestProcessor
	{
		DispatchRuntime dispatch_runtime;
		IChannel reply_or_input;
		IContextChannel context_channel;

		public InputOrReplyRequestProcessor (DispatchRuntime runtime, IChannel replyOrInput)
		{
			Init (runtime, replyOrInput);

			//initialization
			InitializeChain.AddHandler (new InitializingHandler ());

			//processing
			ProcessingChain.AddHandler (new PostReceiveRequestHandler ()).
							AddHandler (new OperationInvokerHandler (replyOrInput));

			//errors
			ErrorChain.AddHandler (new ErrorProcessingHandler (replyOrInput));

			//finalize
			FinalizationChain.AddHandler (new FinalizeProcessingHandler ());
		}

		void Init (DispatchRuntime runtime, IChannel replyOrInput)
		{
			dispatch_runtime = runtime;
			reply_or_input = replyOrInput;
		}

		public void ProcessInput (Message message)
		{
			OperationContext opcx = CreateOperationContext (message);
			ProcessRequest (new MessageProcessingContext (opcx, reply_or_input));
		}

		public void ProcessReply (RequestContext rc)
		{
			OperationContext opcx = CreateOperationContext (rc.RequestMessage);
			opcx.RequestContext = rc;
			ProcessRequest (new MessageProcessingContext (opcx, reply_or_input));
		}

		OperationContext CreateOperationContext (Message incoming)
		{
			IContextChannel contextChannel;
			if (dispatch_runtime.CallbackClientRuntime.CallbackClientType != null) {
#if DISABLE_REAL_PROXY
				var type = ServiceProxyGenerator.CreateCallbackProxyType (dispatch_runtime, dispatch_runtime.CallbackClientRuntime.CallbackClientType);
				contextChannel = (ServiceRuntimeChannel) Activator.CreateInstance (type, new object [] {reply_or_input, dispatch_runtime});
#else
				contextChannel = (IContextChannel) new ClientRealProxy (dispatch_runtime.CallbackClientRuntime.CallbackClientType, new DuplexServiceRuntimeChannel (reply_or_input, dispatch_runtime), true).GetTransparentProxy ();
#endif
			}
			else
				contextChannel = new ServiceRuntimeChannel (reply_or_input, dispatch_runtime);
			contextChannel.Open (); // FIXME: timeout?
			OperationContext opCtx = new OperationContext (contextChannel);
			opCtx.IncomingMessage = incoming;
			opCtx.EndpointDispatcher = dispatch_runtime.EndpointDispatcher;
			context_channel = contextChannel;
			return opCtx;
		}
	}
}
