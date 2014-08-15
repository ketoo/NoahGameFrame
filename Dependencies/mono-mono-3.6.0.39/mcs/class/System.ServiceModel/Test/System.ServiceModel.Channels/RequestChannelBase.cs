//
// RequestChannelBase.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2006 Novell, Inc.  http://www.novell.com
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
using System.IO;
using System.Net;
using System.Net.Security;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Description;
using System.ServiceModel.Security;

namespace MonoTests.System.ServiceModel.Channels
{
	public abstract class RequestChannelBase : ChannelBase, IRequestChannel
	{
		ChannelFactoryBase channel_factory;

		public RequestChannelBase (ChannelFactoryBase factory)
			: base (factory)
		{
			this.channel_factory = factory;
		}

		protected override TimeSpan DefaultCloseTimeout {
			get { return TimeSpan.FromSeconds (5); }
		}

		protected override TimeSpan DefaultOpenTimeout {
			get { return TimeSpan.FromSeconds (5); }
		}

		public abstract EndpointAddress RemoteAddress { get; }

		public abstract Uri Via { get; }

		// Request

		public Message Request (Message message)
		{
			return Request (message, DefaultSendTimeout);
		}

		public abstract Message Request (Message message, TimeSpan timeout);

		public IAsyncResult BeginRequest (Message message, AsyncCallback callback, object state)
		{
			return BeginRequest (message, DefaultSendTimeout, callback, state);
		}

		public abstract IAsyncResult BeginRequest (Message message, TimeSpan timeout, AsyncCallback callback, object state);

		public abstract Message EndRequest (IAsyncResult result);
	}
}
