//
// ProjectStartedEventArgsTest.cs:
//
// Author:
//   Marek Sieradzki (marek.sieradzki@gmail.com)
//
// (C) 2005 Marek Sieradzki
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

using System.Collections;
using Microsoft.Build.Framework;
using NUnit.Framework;

namespace MonoTests.Microsoft.Build.Framework {
	[TestFixture]
	public class ProjectStartedEventArgsTest {
		[Test]
		public void AssignmentTest ()
		{
			ProjectStartedEventArgs psea;
			string message = "message";
			string helpKeyword = "helpKeyword";
			string projectFile = "projectFile";
			string targetNames = "targetNames";
			string [] properties = new string [2] { "a", "b" };
			string [] items = new string [2] { "c", "d" };

			
			psea = new ProjectStartedEventArgs (message, helpKeyword, projectFile, targetNames, properties, items);
			
			Assert.AreEqual (message, psea.Message, "A1");
			Assert.AreEqual (helpKeyword, psea.HelpKeyword, "A2");
			Assert.AreEqual (projectFile, psea.ProjectFile, "A3");
			Assert.AreEqual (targetNames, psea.TargetNames, "A4");
			Assert.AreEqual (properties, psea.Properties, "A5");
			Assert.AreEqual (items, psea.Items, "A6");
		}
	}
}
