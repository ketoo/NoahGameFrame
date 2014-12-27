//
// OdbcDataReaderTest.cs - NUnit Test Cases for testing the
// OdbcDataReader class
//
// Author: 
//      Sureshkumar T (TSureshkumar@novell.com)
// 
// Copyright (c) 2004 Novell Inc., and the individuals listed
// on the ChangeLog entries.
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
using System.Text;
using System.Data;
using System.Data.Odbc;

using NUnit.Framework;

namespace MonoTests.System.Data.Odbc
{

  [TestFixture]
  public class OdbcDataReaderTest : MySqlOdbcBaseClient 
  {
          
          [SetUp]
          public void GetReady () {
                OpenConnection ();
                CreateTestSetup (); // create database & test tables
          }

          [TearDown]
          public void Clean () {
                CleanTestSetup (); // clean test database;
                CloseConnection ();
          }

        /// <summary>
        /// Tests the return value of GetByte method of OdbcDataReader
        /// </summary>
        [Test]
        public void GetByteTest () 
        {
                OdbcDataReader reader = null;
                try {
                        // For this Test, you must create sample table
                        // called test, with a column of name 'col_int'.
                        // and the table with atleast a row with a minimum value for col_int as 0xff
                        // This tries to read a int column using GetByte method
                        OdbcCommand cmd = conn.CreateCommand ();
                        string query = "select col_int from test order by col_int;";
                        cmd.CommandText = query;
                        reader = cmd.ExecuteReader ();
                        if (reader.Read ()) {
                                byte b = reader.GetByte (0); 
                                Assert.AreEqual (0xff, b, "GetByte returns wrong result!");
                        } else // This should not happen while testing
                                Assert.AreEqual (true, true, "test table doens not have a test data!");
                    } finally { // try/catch is necessary to gracefully close connections
                        if (reader != null && reader.IsClosed)
                                reader.Close ();
                        CleanTestSetup ();
                        CloseConnection ();
                  }
        }

        /// <summary>
        /// Tests the return column type of data reader
        /// To test the bugzilla id 49340
        /// </summary>
        [Test]
        public void ColumnDataTypeTest () 
        {
                OdbcCommand dbcmd = conn.CreateCommand ();
                string sql = "SELECT  * from test";
                dbcmd.CommandText = sql;
                IDataReader reader = dbcmd.ExecuteReader ();
                try {
                        Assert.AreEqual ("TinyInt", reader.GetDataTypeName (0), "GetDataTypeName returns invalid Type for column #1");
                        Assert.AreEqual ("VarChar", reader.GetDataTypeName (1), "GetDataTypeName returns invalid Type for column #2");
                        // Test via method GetFieldType.ToString
                        Assert.AreEqual ("System.Byte", reader.GetFieldType (0).ToString (), "GetFieldType returns invalid Type for column #1");
                        Assert.AreEqual ("System.String", reader.GetFieldType (1).ToString (), "GetFieldType returns invalid Type for column #2");

                        // Test via method GetSchemaTable
                        reader = dbcmd.ExecuteReader ();
                        DataTable schemaTable = reader.GetSchemaTable ();
                        Assert.AreEqual (typeof (Byte), schemaTable.Rows [0]["DataType"], "GetSchemaTable.ColumnDataType failes for column #1");
                        Assert.AreEqual (typeof (String), schemaTable.Rows [1]["DataType"], "GetSchemaTable.ColumnDataType failes for column #1");
              } finally {
                        // clean up
                        if (reader != null && !reader.IsClosed)
                                reader.Close ();
                        reader = null;
                        CleanTestSetup ();
                        CloseConnection ();
              }
        }

        [Test]
        public void GetNameTest () 
        {
              OdbcCommand dbcmd = conn.CreateCommand ();
              string sql = "SELECT  * from test";
              dbcmd.CommandText = sql;
              OdbcDataReader reader = dbcmd.ExecuteReader ();
              try {
                      Assert.AreEqual ("pk_tint", reader.GetName (0), "GetName failes ");
              } finally {
                      // clean up
                      if (reader != null && !reader.IsClosed)
                              reader.Close ();
                      reader = null;
                      CleanTestSetup ();
                      CloseConnection ();
              }
      }

      [Test]
      public void GetBytesTest ()
      {
                OdbcCommand cmd = conn.CreateCommand ();
                string sql = "SELECT * FROM test";
                cmd.CommandText = sql;
                OdbcDataReader reader = cmd.ExecuteReader (CommandBehavior.SequentialAccess);
                try {
                        if (reader.Read ()) {
                                // Get By Parts for the column blob
                                int totalsize = 100;
                                int buffsize = 5;
                                int buffstart = 0;
                                long retval = 0;
                                long start = 0;
                                byte [] val = new byte [totalsize];
                                retval = reader.GetBytes (3, start, val, buffstart, buffsize);
                                while (retval == buffsize) {
                                        start += buffsize;
                                        buffstart += buffsize;
                                        retval = reader.GetBytes (3, start, val, buffstart, buffsize);
                                }
                                buffstart += (int) retval;

                                // assemble here.
                                string col = Encoding.Default.GetString (val, 0, buffstart);
                                
                                Assert.AreEqual (39, col.Length, "The assembled value length does not match");
                        }
                } finally {
                        // clean up
                        if (reader != null && !reader.IsClosed)
                                reader.Close ();
                        reader = null;
                        CleanTestSetup ();
                        CloseConnection ();
                }
      }

      [Test]
      public void GetBytesNullBufferTest ()
      {
                OdbcCommand cmd = conn.CreateCommand ();
                string sql = "SELECT * FROM test";
                cmd.CommandText = sql;
                OdbcDataReader reader = cmd.ExecuteReader (CommandBehavior.SequentialAccess);
                try {
                        if (reader.Read ()) {
                                Assert.AreEqual (11, reader.GetBytes (1,0,null,0,0), "GetBytes on a fixed length column does not work!");
                                Assert.AreEqual (39, reader.GetBytes (3,0,null,0,0), "GetBytes with non null column does not work!");
                        }
                        // for null value, length in bytes should return 0
                        if (reader.Read ()) 
                                AssertEquals ("GetBytes with null column does not return -1" , -1, reader.GetBytes (3,0,null,0,0));
                } finally {
                        // clean up
                        if (reader != null && !reader.IsClosed )
                                reader.Close ();
                        reader = null;
                        CleanTestSetup ();
                        CloseConnection ();
                }
      }

      [Test]
      public void GetValueBinaryTest ()
      {
                OdbcCommand cmd = conn.CreateCommand ();
                string sql = "SELECT * FROM test";
                cmd.CommandText = sql;
                OdbcDataReader reader = cmd.ExecuteReader (CommandBehavior.SequentialAccess);
                try {
                        if (reader.Read ()) {
                                object ob = reader.GetValue (3);
                                Assert.AreEqual ("System.Byte[]", ob.GetType ().ToString () , "Type of binary column is wrong!");
                        }
                } finally {
                        // clean up
                        if (reader != null && !reader.IsClosed )
                                reader.Close ();
                        reader = null;
                        CleanTestSetup ();
                        CloseConnection ();
                }
      }

      [Test]
      public void GetDateTimeTest ()
      {
                OdbcCommand cmd = conn.CreateCommand ();
                string sql = "SELECT * FROM test";
                cmd.CommandText = sql;
                OdbcDataReader reader = cmd.ExecuteReader (CommandBehavior.Default);
                try {
                        if (reader.Read ()) {
                                object ob = reader["col_datetime"];
                                Assert.AreEqual ("System.DateTime", ob.GetType ().ToString () , "Type of datetime column is wrong!");
                                ob = reader["col_date"];
                                Assert.AreEqual ("System.DateTime", ob.GetType ().ToString () , "Type of date column is wrong!");
				// FIXME : Once TIME data type is fixed, enable this check
                                //ob = reader["col_time"];
                                //Assert.AreEqual ("System.DateTime", ob.GetType ().ToString () , "Type of time column is wrong!");

				DateTime dt = reader.GetDateTime (4);
				Assert.AreEqual (new DateTime (2004, 8, 22, 0, 0, 0), dt, "DateValue (SQL_TIMESTAMP) is wrong");
				dt = reader.GetDateTime (5);
				Assert.AreEqual (new DateTime (2004, 8, 22, 0, 0, 0), dt, "DateValue (SQL_DATE) is wrong");
				// FIXME : Once TIME data type is fixed, enable this check
				//dt = reader.GetDateTime (7);
				//Assert.AreEqual ("2004-08-22", dt.ToString (), "DateValue is wrong");
                        }
                } finally {
                        // clean up
                        if (reader != null && !reader.IsClosed )
                                reader.Close ();
                        reader = null;
                        CleanTestSetup ();
                        CloseConnection ();
                }
      }


		
      [Test]
      public void NumericTest()
     {
		using(IDbConnection dbConnection = new OdbcConnection
		(connectionString))
		{
			dbConnection.Open();
			IDbCommand dbCommand = dbConnection.CreateCommand();
			//note this will fail if the table already exists, ie if the test has failed.	
			dbCommand.CommandText = "CREATE TABLE NumericTable (NumericField NUMERIC(10) NOT NULL)";
			dbCommand.ExecuteNonQuery();
			dbCommand.CommandText = "INSERT INTO NumericTable (NumericField) VALUES (125)";
			dbCommand.ExecuteNonQuery();
			dbCommand.CommandText = "SELECT * FROM NumericTable";
			using(IDataReader reader = dbCommand.ExecuteReader())
			{
				while(reader.Read()) 
				{
					for(int index = 0; index < reader.FieldCount; index++)
					{
						Object dataValue = reader.GetValue(index);
						Assert.AreEqual("System.Decimal",dataValue.GetType().ToString());
						Assert.AreEqual("125", dataValue.ToString()); 	
				} 
			}
		}

		dbCommand.CommandText = "DROP TABLE NumericTable";
		dbCommand.ExecuteNonQuery();
	   }	
     }


      /// <summary>
      /// This test for the return type &amp; value for GetValue
      /// in case of Odbc Data type TINYINT
      /// </summary>
      [Test]
      public void TinyIntTest ()
      {
                OdbcCommand cmd = conn.CreateCommand ();
                string sql = "SELECT * FROM test";
                cmd.CommandText = sql;
                OdbcDataReader reader = cmd.ExecuteReader (CommandBehavior.SequentialAccess);
                try {
                        if (reader.Read ()) {
                                object ob = reader.GetValue (0);
                                Assert.AreEqual ("System.Byte", ob.GetType ().ToString () , "Type of tinyInt column is wrong!");
                                Assert.AreEqual (1, Convert.ToInt16(ob) , "Value of tinyInt column is wrong!");
                        }
                } finally {
                        // clean up
                        if (reader != null && !reader.IsClosed )
                                reader.Close ();
                        reader = null;
                        CleanTestSetup ();
                        CloseConnection ();
                }
      }


  }
}
