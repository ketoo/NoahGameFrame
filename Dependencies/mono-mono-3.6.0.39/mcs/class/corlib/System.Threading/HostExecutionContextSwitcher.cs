//
// System.Threading.HostExecutionContextSwitcher structure
//
// Author:
//	Sebastien Pouliot  <sebastien@ximian.com>
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

using System.Runtime.ConstrainedExecution;
using System.Runtime.InteropServices;

namespace System.Threading {

	[ComVisible (false)]
	public struct HostExecutionContextSwitcher : IDisposable {

		[MonoTODO]
		public HostExecutionContext CurrentHostExecutionContext {
			get { throw new NotImplementedException (); }
			set { throw new NotImplementedException (); }
		}

		[MonoTODO]
		public HostExecutionContext PreviousHostExecutionContext {
			get { throw new NotImplementedException (); }
			set { throw new NotImplementedException (); }
		}

		[MonoTODO]
		[ReliabilityContract (Consistency.WillNotCorruptState, Cer.MayFail)]
		public void Undo ()
		{
		}

		[MonoTODO]
		public override bool Equals (object o)
		{
			if (o == null)
				return false;
			// TODO
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public override int GetHashCode ()
		{
			return 0;
		}

		void IDisposable.Dispose ()
		{
			Undo ();
		}
	}
}

