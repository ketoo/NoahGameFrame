// Authors:
//   Rafael Mizrahi   <rafim@mainsoft.com>
//   Erez Lotan       <erezl@mainsoft.com>
//   Oren Gurfinkel   <oreng@mainsoft.com>
//   Ofer Borstein
//
// Copyright (c) 2004 Mainsoft Co.
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
using System.Text;
using System.IO;
using System.Data;

namespace MonoTests_System.Data
{
	[TestFixture] public class SyntaxErrorExceptionTest
	{
		[Test] public void Generate()
		{
			Exception tmpEx = new Exception() ;

			DataTable tbl = new DataTable();
			tbl.Columns.Add(new DataColumn("Column"));
			DataColumn dc = new DataColumn();
			dc.Expression = "something"; //invalid expression

			// SyntaxErrorException - Column Expression
			try 
			{
				tbl.Columns[0].Expression = "Colummn +=+ 1"; //invalid expression
				Assert.Fail("SEE1: Columns[0].Expression failed to raise SyntaxErrorException.");
			}
			catch (SyntaxErrorException) {}
			catch (AssertionException) { throw; }
			catch (Exception exc)
			{
				Assert.Fail("SEE2: Columns[0].Expression wrong exception type. Got: " + exc);
			}
			// SyntaxErrorException - Select 
			try 
			{
				tbl.Select("Name += bulshit");
				Assert.Fail("SEE3: Select failed to raise SyntaxErrorException.");
			}
			catch (SyntaxErrorException) {}
			catch (AssertionException) { throw; }
			catch (Exception exc)
			{
				Assert.Fail("SEE4: Select wrong exception type. Got: " + exc);
			}
		}
	}
}
