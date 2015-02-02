//
// System.Web.HttpCacheVaryByParamsTest.cs - Unit tests for System.Web.HttpCacheByParams
//
// Author:
//	Chris Toshok  <toshok@novell.com>
//
// Copyright (C) 2005 Novell, Inc (http://www.novell.com)
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
using System.Web;

using NUnit.Framework;

namespace MonoTests.System.Web {

	[TestFixture]
	public class HttpCacheVaryByParamsTest {

		[Test]
		public void Properties ()
		{
			HttpResponse response = new HttpResponse (Console.Out);
			HttpCacheVaryByParams vary = response.Cache.VaryByParams;

			vary["hi"] = true;
			Assert.IsTrue (vary["hi"], "vary[hi] == true");

			/* make sure setting IgnoreParams to either case doesn't clear the value */
			vary.IgnoreParams = true;
			Assert.IsTrue (vary["hi"], "vary[hi] == true when IgnoreParams == true");

			vary.IgnoreParams = false;
			Assert.IsTrue (vary["hi"], "vary[hi] == true when IgnoreParams == false");

			/* test case sensitivity */
			Assert.IsTrue (vary["Hi"], "vary[Hi] == true");
		}
	}
}
