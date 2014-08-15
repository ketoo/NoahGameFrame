//
// System.Runtime.Remoting.Channels.TransportHeaders.cs
//
// Author: Rodrigo Moya (rodrigo@ximian.com)
//         Dietmar Maurer (dietmar@ximian.com)
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

using System.Collections;

namespace System.Runtime.Remoting.Channels
{
        [Serializable]
	[MonoTODO ("Serialization format not compatible with .NET")]
	[System.Runtime.InteropServices.ComVisible (true)]
	public class TransportHeaders : ITransportHeaders
	{
		Hashtable hash_table;
		
		public TransportHeaders ()
		{
			hash_table = new Hashtable (CaseInsensitiveHashCodeProvider.DefaultInvariant, CaseInsensitiveComparer.DefaultInvariant);
		}

		public object this [object key]
	        {
			get {
				return  hash_table [key];
			}
			
			set {
				hash_table [key] = value;
			}
		}

		public IEnumerator GetEnumerator ()
		{
			return  hash_table.GetEnumerator ();
		}
	}
}
