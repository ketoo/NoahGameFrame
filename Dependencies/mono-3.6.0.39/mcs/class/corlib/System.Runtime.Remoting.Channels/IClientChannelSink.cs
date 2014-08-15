//
// System.Runtime.Remoting.Channels.IClientChannelSink.cs
//
// Author: Duncan Mak (duncan@ximian.com)
//
// 2002 (C) Copyright, Ximian, Inc.
//

//
// Copyright (C) 2004 Novell, Inc (http://www.novell.com)
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

using System.IO;
using System.Runtime.Remoting.Messaging;

namespace System.Runtime.Remoting.Channels {

	[System.Runtime.InteropServices.ComVisible (true)]
	public interface IClientChannelSink : IChannelSinkBase
	{
		IClientChannelSink NextChannelSink { get; }

		void AsyncProcessRequest (IClientChannelSinkStack sinkStack, IMessage msg,
					  ITransportHeaders headers, Stream stream);

		void AsyncProcessResponse (IClientResponseChannelSinkStack sinkStack, object state,
					   ITransportHeaders headers, Stream stream);

		Stream GetRequestStream (IMessage msg, ITransportHeaders headers);

		void ProcessMessage (IMessage msg, ITransportHeaders requestHeaders, Stream requestStream,
				     out ITransportHeaders responseHeaders, out Stream responseStream);
	}
}
