//
// System.ObjectDisposedException.cs
//
// Authors:
//   Paolo Molaro (lupus@ximian.com)
//   Duncan Mak (duncan@ximian.com)
//
// (C) 2001 Ximian, Inc.  http://www.ximian.com
//

//
// Copyright (C) 2004 Novell, Inc (http://www.novell.com)
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

using System.Runtime.Serialization;
using System.Runtime.InteropServices;

namespace System
{
	[Serializable]
	[ComVisible (true)]
	public class ObjectDisposedException : InvalidOperationException
	{
		// Does not override the HResult from InvalidOperationException

		private string obj_name;
		private string msg;

		// Constructors
		public ObjectDisposedException (string objectName)
			: base (Locale.GetText ("The object was used after being disposed."))
		{
			obj_name = objectName;
			msg = Locale.GetText ("The object was used after being disposed.");
		}

		public ObjectDisposedException (string objectName, string message) 
			: base (message)
		{
			obj_name = objectName;
			msg = message;
		}

		public ObjectDisposedException (string message, Exception innerException)
			: base(message, innerException)
		{
		}

		protected ObjectDisposedException (SerializationInfo info, StreamingContext context)
			: base (info, context)
		{
			obj_name = info.GetString ("ObjectName");
		}

		// Properties
		public override string Message {
			get { return msg; }
		}

		public string ObjectName {
			get { return obj_name; }
		}

		public override void GetObjectData (SerializationInfo info, StreamingContext context)
		{
			base.GetObjectData (info, context);
			info.AddValue ("ObjectName", obj_name);
		}
	}
}
