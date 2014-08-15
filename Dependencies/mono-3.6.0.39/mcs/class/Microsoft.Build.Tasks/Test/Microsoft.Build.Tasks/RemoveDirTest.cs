//
// RemoveDirTest.cs
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
using System.Collections;
using System.IO;
using Microsoft.Build.BuildEngine;
using Microsoft.Build.Framework;
using Microsoft.Build.Tasks;
using Microsoft.Build.Utilities;
using NUnit.Framework;

namespace MonoTests.Microsoft.Build.Tasks {

	[TestFixture]
	public class RemoveDirTest {

		[SetUp]
		public void CreateDir ()
		{
			string path = Path.Combine (Path.Combine ("Test", "resources"), "RemoveDir");
			Directory.CreateDirectory (path);
		}

		[TearDown]
		public void RemoveDirectories ()
		{
			string path = Path.Combine (Path.Combine ("Test", "resources"), "RemoveDir");
			Directory.Delete (path, true);
		}

		[Test]
		public void TestAssignment ()
		{
			RemoveDir rd = new RemoveDir ();

			rd.Directories = new ITaskItem [] { new TaskItem ("A")};

			Assert.AreEqual ("A", rd.Directories [0].ItemSpec, "A1");
		}

		[Test]
		public void TestExecution1 ()
		{
			Engine engine;
			Project project;
			string path = Path.Combine (Path.Combine ("Test", "resources"), "RemoveDir");

			string documentString = @"
                                <Project xmlns=""http://schemas.microsoft.com/developer/msbuild/2003"">
					<ItemGroup>
						<Dir Include='Test\resources\RemoveDir\A\B' />
						<Dir Include='Test\resources\RemoveDir\A' />
						<Dir Include='Test\resources\RemoveDir\C' />
					</ItemGroup>
					<Target Name='1'>
						<RemoveDir Directories='@(Dir)'>
							<Output
								TaskParameter='RemovedDirectories'
								ItemName='Out'
							/>
						</RemoveDir>
					</Target>
				</Project>
			";

			Directory.CreateDirectory (Path.Combine (path, "A"));
			Directory.CreateDirectory (Path.Combine (Path.Combine (path, "A"), "B"));
			Directory.CreateDirectory (Path.Combine (path, "C"));

			engine = new Engine (Consts.BinPath);
			project = engine.CreateNewProject ();
			project.LoadXml (documentString);
			Assert.IsTrue (project.Build ("1"), "A1");

			Assert.AreEqual (0, Directory.GetDirectories (path).Length, "A2");

			BuildItemGroup output = project.GetEvaluatedItemsByName ("Out");
			Assert.AreEqual (Path.Combine (Path.Combine (path, "A"), "B"), output [0].FinalItemSpec, "A3");
			Assert.AreEqual (Path.Combine (path, "A"), output [1].FinalItemSpec, "A4");
			Assert.AreEqual (Path.Combine (path, "C"), output [2].FinalItemSpec, "A5");

		}
	}
}
