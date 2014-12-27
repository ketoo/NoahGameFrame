//
// System.Web.UI.ThemeableAttribute
//
// Authors:
//	Sanjay Gupta (gsanjay@novell.com)
//
// (C) 2004 Novell, Inc. (http://www.novell.com)
// Copyright (C) 2005-2010 Novell, Inc (http://www.novell.com)
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
using System.Security.Permissions;

namespace System.Web.UI 
{
	// CAS - no InheritanceDemand here as the class is sealed
	[AspNetHostingPermission (SecurityAction.LinkDemand, Level = AspNetHostingPermissionLevel.Minimal)]
	// attributes
	[AttributeUsage (AttributeTargets.Class | AttributeTargets.Property, AllowMultiple = false, Inherited = true)]
	public sealed class ThemeableAttribute : Attribute {

		bool themeable;
		public ThemeableAttribute (bool themeable) 
		{
			this.themeable = themeable;
		}

		public static readonly ThemeableAttribute Default = new ThemeableAttribute (true);

		public static readonly ThemeableAttribute No = new ThemeableAttribute (false);

		public static readonly ThemeableAttribute Yes = new ThemeableAttribute (true);
		
		public bool Themeable { 
			get { return themeable; } 
		}

		public override bool Equals (object obj)
		{
			if (obj != null && obj is ThemeableAttribute) {
				ThemeableAttribute ta = (ThemeableAttribute) obj;
				return (this.themeable == ta.themeable);
			}
			return false;
		}

		public override int GetHashCode ()
		{
			return this.themeable.GetHashCode ();
		}

		public override bool IsDefaultAttribute()
		{
			return Equals (Default);
		}

		public static bool IsObjectThemeable (object obj)
		{
			return IsTypeThemeable (obj.GetType ());
		}

		public static bool IsTypeThemeable (Type type)
		{
			Object [] attributes = type.GetCustomAttributes (typeof (ThemeableAttribute), false);
			if (attributes.Length != 0) {
				foreach (Attribute attrib in attributes)
					if (attrib is ThemeableAttribute)
						return true;
			}
			return false;
		}
	}
}
