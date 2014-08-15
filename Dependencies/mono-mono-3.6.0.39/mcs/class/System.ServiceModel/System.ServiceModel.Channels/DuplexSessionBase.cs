// 
// DuplexSessionBase.cs
// 
// Author: 
//	Atsushi Enomoto  <atsushi@ximian.com>
// 
// Copyright (C) 2009 Novell, Inc (http://www.novell.com)
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
using System.Net.Sockets;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.ServiceModel.Channels;
using System.Text;
using System.Xml;

namespace System.ServiceModel.Channels
{
	internal abstract class DuplexSessionBase : IDuplexSession
	{
		delegate void AsyncHandler (TimeSpan timeout);
		AsyncHandler async_method;

		protected DuplexSessionBase ()
		{
			async_method = new AsyncHandler (CloseOutputSession);
			Id = new UniqueId ().ToString ();
		}

		public abstract TimeSpan DefaultCloseTimeout { get; }

		public virtual string Id { get; private set; }

		public abstract void Close (TimeSpan timeout);

		public virtual void CloseOutputSession ()
		{
			CloseOutputSession (DefaultCloseTimeout);
		}

		public virtual void CloseOutputSession (TimeSpan timeout)
		{
			Close (timeout);
		}

		public virtual IAsyncResult BeginCloseOutputSession (AsyncCallback callback, object state)
		{
			return BeginCloseOutputSession (DefaultCloseTimeout, callback, state);
		}

		public virtual IAsyncResult BeginCloseOutputSession (TimeSpan timeout, AsyncCallback callback, Object state)
		{
			return async_method.BeginInvoke (timeout, callback, state);
		}

		public virtual void EndCloseOutputSession (IAsyncResult result)
		{
			async_method.EndInvoke (result);
		}
	}
}
