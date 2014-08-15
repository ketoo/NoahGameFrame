// Transport Security Layer (TLS)
// Copyright (c) 2003-2004 Carlos Guzman Alvarez
// Copyright (C) 2006 Novell, Inc (http://www.novell.com)
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
using System.Security.Cryptography;

using Mono.Security.Cryptography;

namespace Mono.Security.Protocol.Tls.Handshake.Server
{
	internal class TlsServerFinished : HandshakeMessage
	{
		#region Constructors

		public TlsServerFinished(Context context) 
			: base(context, HandshakeType.Finished)
		{
		}

		#endregion

		#region Protected Methods

		static private byte[] Ssl3Marker = new byte [4] { 0x53, 0x52, 0x56, 0x52 };

		protected override void ProcessAsSsl3()
		{
			// Compute handshake messages hashes
			HashAlgorithm hash = new SslHandshakeHash(this.Context.MasterSecret);

			byte[] data = this.Context.HandshakeMessages.ToArray ();
			hash.TransformBlock (data, 0, data.Length, data, 0);
			hash.TransformBlock (Ssl3Marker, 0, Ssl3Marker.Length, Ssl3Marker, 0);
			// hack to avoid memory allocation
			hash.TransformFinalBlock (CipherSuite.EmptyArray, 0, 0);

			this.Write(hash.Hash);
		}

		protected override void ProcessAsTls1()
		{
			// Compute handshake messages hash
			HashAlgorithm hash = new MD5SHA1();
			byte[] data = this.Context.HandshakeMessages.ToArray ();
			byte[] digest = hash.ComputeHash (data, 0, data.Length);

			// Write message
			this.Write(this.Context.Current.Cipher.PRF(
				this.Context.MasterSecret, "server finished", digest, 12));
		}

		#endregion
	}
}
