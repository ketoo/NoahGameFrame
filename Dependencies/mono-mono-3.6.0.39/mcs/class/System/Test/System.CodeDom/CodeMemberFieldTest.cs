//
// CodeMemberFieldTest.cs - NUnit Test Cases for System.CodeDom.CodeMemberField
//
// Authors:
//   Gert Driesen (drieseng@users.sourceforge.net)
//
// (C) 2005 Novell
//

using NUnit.Framework;
using System.CodeDom;

namespace MonoTests.System.CodeDom
{
	[TestFixture]
	public class CodeMemberFieldTest
	{
		[Test]
		public void DefaultType ()
		{
			CodeMemberField fld = new CodeMemberField ();
			Assert.IsNotNull (fld.Type, "#1");
			Assert.AreEqual (new CodeTypeReference (string.Empty).BaseType, fld.Type.BaseType, "#2");
		}
	}
}
