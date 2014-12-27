//
// BaseValidatorCas.cs 
//	- CAS unit tests for System.Web.UI.WebControls.BaseValidator
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
using System.Web.UI.WebControls;

using MonoTests.System.Web.UI.WebControls;

namespace MonoCasTests.System.Web.UI.WebControls {

	[TestFixture]
	[Category ("CAS")]
	public class BaseValidatorCas {

		// note: we do not inherit from AspNetHostingMinimal because
		// BaseValidator is an abstract class

		[SetUp]
		public virtual void SetUp ()
		{
			if (!SecurityManager.SecurityEnabled)
				Assert.Ignore ("SecurityManager.SecurityEnabled is OFF");
		}

		[Test]
		[PermissionSet (SecurityAction.Deny, Unrestricted = true)]
		public void Deny_Unrestricted ()
		{
			BaseValidatorTest unit = new BaseValidatorTest ();
			unit.ViewState ();
			unit.ControlPropertiesValid1 ();
			unit.CustomDescriptor ();
			unit.NoCustomDescriptor ();
		}

		[Test]
		[AspNetHostingPermission (SecurityAction.Deny, Level = AspNetHostingPermissionLevel.Minimal)]
		[ExpectedException (typeof (TargetInvocationException))]
		public void ValidationProperty_Deny_Minimal ()
		{
			BaseValidatorTest unit = new BaseValidatorTest ();
			unit.ValidationProperty ();
			// note: this is a failing security check on reflection,
			// the SecurityException is the InnerException of the 
			// TargetInvocationException
		}

		[Test]
		[AspNetHostingPermission (SecurityAction.PermitOnly, Level = AspNetHostingPermissionLevel.Minimal)]
		public void ValidationProperty_PermitOnly_Minimal ()
		{
			BaseValidatorTest unit = new BaseValidatorTest ();
			unit.ValidationProperty ();
		}

		[Test]
		[AspNetHostingPermission (SecurityAction.Deny, Level = AspNetHostingPermissionLevel.Minimal)]
		[ExpectedException (typeof (TargetInvocationException))]
		public void GetControlValidationValue_Deny_Minimal ()
		{
			BaseValidatorTest unit = new BaseValidatorTest ();
			unit.GetControlValidationValue ();
			// note: this is a failing security check on reflection,
			// the SecurityException is the InnerException of the 
			// TargetInvocationException
		}

		[Test]
		[AspNetHostingPermission (SecurityAction.PermitOnly, Level = AspNetHostingPermissionLevel.Minimal)]
		public void GetControlValidationValue_PermitOnly_Minimal ()
		{
			BaseValidatorTest unit = new BaseValidatorTest ();
			unit.GetControlValidationValue ();
		}
	}
}
