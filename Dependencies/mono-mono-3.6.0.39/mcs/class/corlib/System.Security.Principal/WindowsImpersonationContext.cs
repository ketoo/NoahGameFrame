//
// System.Security.Principal.WindowsImpersonationContext
//
// Authors:
//      Gonzalo Paniagua Javier (gonzalo@ximian.com)
//	Sebastien Pouliot  (sebastien@ximian.com)
//
// (C) 2002 Ximian, Inc (http://www.ximian.com)
// Copyright (C) 2004-2005 Novell, Inc (http://www.novell.com)
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

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;

namespace System.Security.Principal {

	[ComVisible (true)]
	public class WindowsImpersonationContext : IDisposable {

		private IntPtr _token;
		private bool undo;

		internal WindowsImpersonationContext (IntPtr token)
		{
			// we get a copy to control it's lifetime
			_token = DuplicateToken (token);
			if (!SetCurrentToken (token)) {
				throw new SecurityException ("Couldn't impersonate token.");
			}
			undo = false;
		}
		[ComVisible (false)]
		public void Dispose ()
		{
			if (!undo) {
				Undo ();
			}
		}
		
		[ComVisible (false)]
		protected virtual void Dispose (bool disposing)
		{
			if (!undo) {
				Undo ();
			}
			if (disposing){
				// If we are explicitly disposed, we can avoid finalization.
				GC.SuppressFinalize (this);
			}
		}

		public void Undo ()
		{
			if (!RevertToSelf ()) {
				CloseToken (_token);
				throw new SecurityException ("Couldn't switch back to original token.");
			}
			CloseToken (_token);
			undo = true;
			GC.SuppressFinalize (this);
		}

		// see mono/mono/metadata/security.c for implementation

		[MethodImplAttribute (MethodImplOptions.InternalCall)]
		private extern static bool CloseToken (IntPtr token);

		[MethodImplAttribute (MethodImplOptions.InternalCall)]
		private extern static IntPtr DuplicateToken (IntPtr token);

		[MethodImplAttribute (MethodImplOptions.InternalCall)]
		private extern static bool SetCurrentToken (IntPtr token);

		[MethodImplAttribute (MethodImplOptions.InternalCall)]
		private extern static bool RevertToSelf ();
	}
}
