//
// System.Security.AccessControl.ObjectAuditRule implementation
//
// Authors:
//	Dick Porter  <dick@ximian.com>
//	Atsushi Enomoto  <atsushi@ximian.com>
//
// Copyright (C) 2006-2007 Novell, Inc (http://www.novell.com)
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

using System.Security.Principal;

namespace System.Security.AccessControl
{
	public abstract class ObjectAuditRule : AuditRule
	{
		protected ObjectAuditRule (IdentityReference identity,
					   int accessMask, bool isInherited,
					   InheritanceFlags inheritanceFlags,
					   PropagationFlags propagationFlags,
					   Guid objectType,
					   Guid inheritedObjectType,
					   AuditFlags auditFlags)
			: base (identity, accessMask, isInherited, inheritanceFlags, propagationFlags, auditFlags)
		{
			object_type = objectType;
			inherited_object_type = inheritedObjectType;
		}

		Guid inherited_object_type, object_type;

		public Guid InheritedObjectType {
			get { return inherited_object_type; }
		}
		
		public ObjectAceFlags ObjectFlags {
			get {
				ObjectAceFlags ret = ObjectAceFlags.None;
				if (object_type != Guid.Empty)
					ret |= ObjectAceFlags.ObjectAceTypePresent;
				if (inherited_object_type != Guid.Empty)
					ret |= ObjectAceFlags.InheritedObjectAceTypePresent;
				return ret;
			}
		}
		
		public Guid ObjectType {
			get { return object_type; }
		}
	}
}

