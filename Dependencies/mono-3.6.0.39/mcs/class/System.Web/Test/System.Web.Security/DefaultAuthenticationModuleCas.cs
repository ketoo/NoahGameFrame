//
// DefaultAuthenticationModuleCas.cs 
//	- CAS unit tests for System.Web.Security.DefaultAuthenticationModule
//
// Author:
//	Sebastien Pouliot  <sebastien@ximian.com>
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

using NUnit.Framework;

using System;
using System.Reflection;
using System.Security;
using System.Security.Permissions;
using System.Web;
using System.Web.Security;

namespace MonoCasTests.System.Web.Security {

	[TestFixture]
	[Category ("CAS")]
	public class DefaultAuthenticationModuleCas : AspNetHostingMinimal {

		private HttpApplication app;
		private DefaultAuthenticationModule module;

		[TestFixtureSetUp]
		public void FixtureSetUp ()
		{
			app = new HttpApplication ();
			module = new DefaultAuthenticationModule ();
		}

		[Test]
		[SecurityPermission (SecurityAction.Deny, UnmanagedCode = true)]
		[ExpectedException (typeof (SecurityException))]
		public void Constructor_Deny_UnmanagedCode ()
		{
			new DefaultAuthenticationModule ();
		}

		[Test]
		[SecurityPermission (SecurityAction.PermitOnly, UnmanagedCode = true)]
		public void Constructor_PermitOnly_UnmanagedCode ()
		{
			new DefaultAuthenticationModule ();
		}

		private void Authenticate (object sender, DefaultAuthenticationEventArgs e)
		{
		}

		[Test]
		[PermissionSet (SecurityAction.Deny, Unrestricted = true)]
		public void Module ()
		{
			// only the ctor requires UnmanagedCode
			module.Init (app);
			module.Authenticate += new DefaultAuthenticationEventHandler (Authenticate);
			module.Authenticate -= new DefaultAuthenticationEventHandler (Authenticate);
			module.Dispose (); // but doesn't implement IDisposable
		}

		// LinkDemand

		[SecurityPermission (SecurityAction.Assert, UnmanagedCode = true)]
		public override object CreateControl (SecurityAction action, AspNetHostingPermissionLevel level)
		{
			return base.CreateControl (action, level);
		}

		public override Type Type {
			get { return typeof (DefaultAuthenticationModule); }
		}
	}
}
