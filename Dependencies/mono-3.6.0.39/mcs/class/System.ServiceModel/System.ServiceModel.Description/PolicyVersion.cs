﻿//
// PolicyVersion.cs
//
// Author:
//	Igor Zelmanovich <igorz@mainsoft.com>
//
// Copyright (C) 2008 Mainsoft, Inc.  http://www.mainsoft.com
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
using System.Text;

namespace System.ServiceModel.Description
{
	public sealed class PolicyVersion
	{
		static PolicyVersion _policy12 = new PolicyVersion ("http://schemas.xmlsoap.org/ws/2004/09/policy");
		static PolicyVersion _policy15 = new PolicyVersion ("http://www.w3.org/ns/ws-policy");

		readonly string _namespace;

		private PolicyVersion (string @namespace) {
			_namespace = @namespace;
		}

		public static PolicyVersion Default {
			get { return _policy12; }
		}

		public static PolicyVersion Policy12 {
			get { return _policy12; }
		}

		public static PolicyVersion Policy15 {
			get { return _policy15; }
		}

		public string Namespace {
			get { return _namespace; }
		}

		public override string ToString () {
			return Namespace;
		}
	}
}
