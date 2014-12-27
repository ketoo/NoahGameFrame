//
// System.Management.ManagementEventWatcher
//
// Author:
//	Atsushi Enomoto (atsushi@ximian.com)
//
// Copyright (C) 2007 Novell, Inc (http://www.novell.com)
//

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

using System.ComponentModel;

namespace System.Management
{
	[ToolboxItem (true)]
	public class ManagementEventWatcher : Component
	{
		[MonoTODO]
		public ManagementEventWatcher ()
		{
		}

		[MonoTODO]
		public ManagementEventWatcher (EventQuery query)
			: this (null, query)
		{
		}

		[MonoTODO]
		public ManagementEventWatcher (string query)
			: this (new EventQuery (query))
		{
		}

		[MonoTODO]
		public ManagementEventWatcher (string scope, string query)
			: this (scope, query, new EventWatcherOptions ())
		{
		}

		[MonoTODO]
		public ManagementEventWatcher (ManagementScope scope, EventQuery query)
			: this (scope, query, new EventWatcherOptions ())
		{
		}

		[MonoTODO]
		public ManagementEventWatcher (string scope, string query, EventWatcherOptions options)
			: this (new ManagementScope (scope), new EventQuery (query), options)
		{
		}

		[MonoTODO]
		public ManagementEventWatcher (ManagementScope scope, EventQuery query, EventWatcherOptions options)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		~ManagementEventWatcher ()
		{
			throw new NotImplementedException ();
		}

		public event EventArrivedEventHandler EventArrived;
		public event StoppedEventHandler Stopped;

		[MonoTODO]
		public EventWatcherOptions Options {
			get { throw new NotImplementedException (); }
			set { throw new NotImplementedException (); }
		}

		[MonoTODO]
		public EventQuery Query {
			get { throw new NotImplementedException (); }
			set { throw new NotImplementedException (); }
		}

		[MonoTODO]
		public ManagementScope Scope {
			get { throw new NotImplementedException (); }
			set { throw new NotImplementedException (); }
		}

		[MonoTODO]
		public void Start ()
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public void Stop ()
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		public ManagementBaseObject WaitForNextEvent ()
		{
			throw new NotImplementedException ();
		}
	}
}

