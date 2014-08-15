﻿//
// IDataServiceHost.cs
//
// Author:
//   Eric Maupin  <me@ermau.com>
//
// Copyright (c) 2009 Eric Maupin (http://www.ermau.com)
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

namespace System.Data.Services {
	public interface IDataServiceHost {
		Uri AbsoluteRequestUri { get; }
		Uri AbsoluteServiceUri { get; }
		string RequestAccept { get; }
		string RequestAcceptCharSet { get; }
		string RequestContentType { get; }
		string RequestHttpMethod { get; }
		string RequestIfMatch { get; }
		string RequestIfNoneMatch { get; }
		Stream RequestStream { get; }
		string RequestVersion { get; }
		string ResponseCacheControl { get; set; }
		string ResponseContentType { get; set; }
		string ResponseETag { get; set; }
		string ResponseLocation { get; set; }
		int ResponseStatusCode { get; set; }
		string RequestMaxVersion { get; }
		Stream ResponseStream { get; }
		string ResponseVersion { get; set; }

		string GetQueryStringItem (string item);
		void ProcessException (HandleExceptionArgs args);
	}
}