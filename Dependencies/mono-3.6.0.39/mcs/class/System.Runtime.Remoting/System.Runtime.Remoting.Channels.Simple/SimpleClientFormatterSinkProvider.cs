//
// System.Runtime.Remoting.Channels.Simple.SimpleClientFormatterSinkProvider.cs
//
// Author: Dietmar Maurer (dietmar@ximian.com)
//
// 2002 (C) Copyright, Ximian, Inc.
//

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

using System.Collections;

namespace System.Runtime.Remoting.Channels.Simple
{
	public class SimpleClientFormatterSinkProvider :
		IClientFormatterSinkProvider, IClientChannelSinkProvider
	{
		IClientChannelSinkProvider next = null;

		// add any sink properties here (private fields)
		
		public SimpleClientFormatterSinkProvider ()
		{
			// nothing to do
		}

		public SimpleClientFormatterSinkProvider (IDictionary properties,
							  ICollection providerData)
	        {
			// copy the contained properties to private fields
			
			// add a check that there is no providerData 
		}

		public IClientChannelSinkProvider Next
		{
			get {
				return next;
			}
			
			set {
				next = value;
			}
		}

		public IClientChannelSink CreateSink (IChannelSender channel,
						      string url,
						      object remoteChannelData)
		{
			IClientChannelSink next_sink = null;
			SimpleClientFormatterSink result;
			
			if (next != null)
				next_sink = next.CreateSink (channel, url, remoteChannelData);
			
			result = new SimpleClientFormatterSink (next_sink);

			// set properties on the newly creates sink

			return result;
		}		
	}
}
