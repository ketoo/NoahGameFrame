
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
using System.Runtime.InteropServices;
using System.Text;
using System.Runtime.Serialization;

namespace IBM.Data.DB2
{


	[Serializable]
	public sealed class DB2Exception : Exception
	{
		private DB2ErrorCollection errors;
		private string message;


		public DB2ErrorCollection Errors
		{
			get
			{
				return errors;
			}
		}
		public override string Message
		{
			get
			{
				if(errors.Count > 0)
				{
					string result = "";
					for(int i = 0; i < errors.Count; i++)
					{
						if(i > 0)
						{
							result += " ";
						}
						result += "ERROR [" + errors[i].SQLState + "] " + errors[i].Message;
					}
					if(message != null)
					{
						result += " " + message;
					}
					return result;
				}
				return "No error information";
			}
		}

		private DB2Exception(SerializationInfo si, StreamingContext sc)
		{
			throw new NotImplementedException();
		}
		public override void GetObjectData(SerializationInfo si, StreamingContext context)
		{
			throw new NotImplementedException();
		}

		internal DB2Exception(short sqlHandleType, IntPtr sqlHandle, string message)
		{	
			this.message = message;
			errors = new DB2ErrorCollection(sqlHandleType, sqlHandle);


		}
	}
}
