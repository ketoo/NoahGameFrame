// 
// System.Web.Services.Protocols.HttpPostClientProtocol.cs
//
// Author:
//   Tim Coleman (tim@timcoleman.com)
//
// Copyright (C) Tim Coleman, 2002
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

using System.Net;
using System.Web.Services;

namespace System.Web.Services.Protocols {
	public class HttpPostClientProtocol : HttpSimpleClientProtocol {

		#region Constructors

		public HttpPostClientProtocol () 
		{
			TypeStub = (HttpSimpleTypeStubInfo) TypeStubManager.GetTypeStub (GetType(), "HttpPost");
		}
		
		#endregion // Constructors

		#region Methods

		protected override WebRequest GetWebRequest (Uri uri)
		{
			if (null == uri)
				throw new InvalidOperationException ("The uri parameter is a null reference.");
			if (String.Empty == uri.ToString ())
				throw new InvalidOperationException ("The uri parameter has a length of zero.");

			WebRequest request = WebRequest.Create (uri);
			request.Method = "POST";
			return request;
		}

		#endregion // Methods
	}
}
