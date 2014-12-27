//
// ProjectElement.cs
//
// Author:
//   Marek Sieradzki (marek.sieradzki@gmail.com)
//
// (C) 2006 Marek Sieradzki
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

using System;
using System.Xml;
using Microsoft.Build.BuildEngine;
using NUnit.Framework;

namespace MonoTests.Microsoft.Build.BuildEngine.Various {
	[TestFixture]
	public class ProjectElement {
		// Clones a project by reloading from original.Xml
		private Project CloneProject (Project original)
		{
			Project clone;
			
			clone = original.ParentEngine.CreateNewProject ();
			clone.LoadXml (original.Xml);

			return clone;
		}

		[Test]
		public void TestDefaultTargetsAttribute ()
		{
			Engine engine;
			Project proj;
			Project cproj;
			string documentString = @"
			<Project xmlns=""http://schemas.microsoft.com/developer/msbuild/2003"" DefaultTargets=""Build;Compile"">
			</Project>
			";
			
			engine = new Engine (Consts.BinPath);
			proj = engine.CreateNewProject ();
			Assert.AreEqual (String.Empty, proj.FullFileName, "A1");

			proj.LoadXml (documentString);
			Assert.AreEqual (String.Empty, proj.FullFileName, "A2");
			proj.DefaultTargets = "Build";
			Assert.AreEqual ("Build", proj.DefaultTargets, "A3");
			cproj = CloneProject (proj);
			Assert.AreEqual (proj.DefaultTargets, cproj.DefaultTargets, "A4");
		}

		[Test]
		[ExpectedException (typeof (InvalidProjectFileException))]
		public void TestRootElement ()
		{
			Engine engine;
			Project proj;
			string documentString = @"
			<something>
			</something>
			";

			engine = new Engine (Consts.BinPath);
			proj = engine.CreateNewProject ();
			proj.LoadXml (documentString);
		}
	}
}
