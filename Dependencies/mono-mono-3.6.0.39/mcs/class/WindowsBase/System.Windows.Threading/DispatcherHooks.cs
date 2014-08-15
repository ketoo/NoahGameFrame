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
// Copyright (c) 2006 Novell, Inc. (http://www.novell.com)
//
// Authors:
//	Miguel de Icaza (miguel@novell.com)
//
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Threading;

namespace System.Windows.Threading {

	public sealed class DispatcherHooks {
		Dispatcher owner;

		internal DispatcherHooks (Dispatcher owner)
		{
			this.owner = owner;
		}
		
		public event EventHandler DispatcherInactive;
		public event DispatcherHookEventHandler OperationAborted;
		public event DispatcherHookEventHandler OperationCompleted;
		public event DispatcherHookEventHandler OperationPosted;
		public event DispatcherHookEventHandler OperationPriorityChanged;

		internal void EmitOperationPosted (DispatcherOperation op)
		{
			DispatcherHookEventHandler posted = OperationPosted;
			if (posted != null)
				posted (owner, new DispatcherHookEventArgs (op));
		}

		internal void EmitOperationCompleted (DispatcherOperation op)
		{
			DispatcherHookEventHandler completed = OperationCompleted;
			if (completed != null)
				completed (owner, new DispatcherHookEventArgs (op));
		}

		internal void EmitOperationAborted (DispatcherOperation op)
		{
			DispatcherHookEventHandler aborted = OperationAborted;
			if (aborted != null)
				aborted (owner, new DispatcherHookEventArgs (op));
		}

		internal void EmitOperationPriorityChanged (DispatcherOperation op)
		{
			DispatcherHookEventHandler prio = OperationPriorityChanged;
			if (prio != null)
				prio (owner, new DispatcherHookEventArgs (op));
		}

		internal void EmitInactive ()
		{
			EventHandler inactive = DispatcherInactive;
			if (inactive != null)
				inactive (owner, EventArgs.Empty);
		}
	}
}
