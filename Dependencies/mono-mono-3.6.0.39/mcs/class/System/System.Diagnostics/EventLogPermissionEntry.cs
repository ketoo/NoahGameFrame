//
// System.Diagnostics.EventLogPermissionEntry.cs
//
// Authors:
//   Jonathan Pryor (jonpryor@vt.edu)
//   Andreas Nahr (ClassDevelopment@A-SoftTech.com)
//
// (C) 2002
// (C) 2003 Andreas Nahr
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

using System.Security.Permissions;

namespace System.Diagnostics {

	[Serializable]
	public class EventLogPermissionEntry {

		private EventLogPermissionAccess permissionAccess;
		private string machineName;

		public EventLogPermissionEntry (EventLogPermissionAccess permissionAccess, string machineName)
		{
			ResourcePermissionBase.ValidateMachineName (machineName);

			this.permissionAccess = permissionAccess;
			this.machineName = machineName;
		}

		public string MachineName {
			get { return machineName; }
		}

		public EventLogPermissionAccess PermissionAccess {
			get { return permissionAccess; }
		}

		internal ResourcePermissionBaseEntry CreateResourcePermissionBaseEntry ()
		{
			return new ResourcePermissionBaseEntry ((int) permissionAccess, new string[] { machineName });
		} 
	}
}

