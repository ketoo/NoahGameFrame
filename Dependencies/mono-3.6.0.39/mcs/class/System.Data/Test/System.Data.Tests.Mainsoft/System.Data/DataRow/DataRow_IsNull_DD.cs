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
using System.Data;

using GHTUtils;
using GHTUtils.Base;

namespace tests.system_data_dll.System_Data
{
[TestFixture] public class DataRow_IsNull_DD : GHTBase
{
	[Test] public void Main()
	{
		DataRow_IsNull_DD tc = new DataRow_IsNull_DD();
		Exception exp = null;
		try
		{
			tc.BeginTest("DataRow_IsNull_DD");
			tc.run();
		}
		catch(Exception ex)
		{
			exp = ex;
		}
		finally
		{
			tc.EndTest(exp);
		}
	}

	//Activate This Construntor to log All To Standard output
	//public TestClass():base(true){}

	//Activate this constructor to log Failures to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, false){}


	//Activate this constructor to log All to a log file
	//public TestClass(System.IO.TextWriter tw):base(tw, true){}

	//BY DEFAULT LOGGING IS DONE TO THE STANDARD OUTPUT ONLY FOR FAILURES

	public void run()
	{
		Exception exp = null;

		DataTable dt = new DataTable(); 
		DataColumn dc0 = new DataColumn("Col0",typeof(int));
		DataColumn dc1 = new DataColumn("Col1",typeof(int));
		dt.Columns.Add(dc0);
		dt.Columns.Add(dc1);
		dt.Rows.Add(new object[] {1234});
		DataRow dr = dt.Rows[0];
			

		try
		{
			BeginCase("IsNull - col0 Current");
			Compare(dr.IsNull(dc0,DataRowVersion.Current) , false);
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}

		try
		{
			BeginCase("IsNull - col1 Current");
			Compare(dr.IsNull(dc1,DataRowVersion.Current) , true);
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}

		try
		{
			BeginCase("IsNull - col0 Default");
			Compare(dr.IsNull(dc0,DataRowVersion.Default) , false);
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}
		try
		{
			BeginCase("IsNull - col1 Default");
			Compare(dr.IsNull(dc1,DataRowVersion.Default) , true);
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}

		dr.BeginEdit();
		dr[0] = 9; //Change value, Create RowVersion Proposed

		try
		{
			BeginCase("IsNull - col0 Proposed");
			Compare(dr.IsNull(dc0,DataRowVersion.Proposed) , false);
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}
		try
		{
			BeginCase("IsNull - col1 Proposed");
			Compare(dr.IsNull(dc1,DataRowVersion.Proposed) , true);
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}

		dr.AcceptChanges();
		dr.Delete();

		try
		{
			BeginCase("IsNull - col0 Original");
			Compare(dr.IsNull(dc0,DataRowVersion.Original) , false);
		}
		catch(Exception ex)	{exp = ex;}
		finally	{EndCase(exp); exp = null;}
	

	}
}
}