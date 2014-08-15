//
// System.Runtime.Remoting.Services.EnterpriseServicesHelper.cs
//
// Author: Lluis Sanchez Gual (lluis@ximian.com)
//
// 2003 (C) Copyright, Novell, Inc.
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

using System.Runtime.Remoting;
using System.Runtime.Remoting.Messaging;
using System.Runtime.Remoting.Activation;
using System.Runtime.Remoting.Proxies;
using System.Runtime.InteropServices;

namespace System.Runtime.Remoting.Services 
{
	[ComVisible (true)]
	public sealed class EnterpriseServicesHelper
	{
		public EnterpriseServicesHelper ()
		{
		}
		
		[ComVisible (true)]
		public static IConstructionReturnMessage CreateConstructionReturnMessage (IConstructionCallMessage ctorMsg, MarshalByRefObject retObj)
		{
			return new ConstructionResponse (retObj, null, ctorMsg);
		}

		[MonoTODO]
		public static void SwitchWrappers (RealProxy oldcp, RealProxy newcp)
		{
			throw new NotSupportedException ();
		}
		
		[MonoTODO]
		public static object WrapIUnknownWithComObject (IntPtr punk)
		{
			throw new NotSupportedException ();
		}
	}
}
