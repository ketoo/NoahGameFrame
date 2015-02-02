//
// System.Web.UI.FilterableAttribute
//
// Authors:
//	Sanjay Gupta (gsanjay@novell.com)
//
// (C) 2004-2010 Novell, Inc. (http://www.novell.com)
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
using System;
using System.ComponentModel;

namespace System.Web.UI {
	[AttributeUsage (AttributeTargets.Class | AttributeTargets.Property, AllowMultiple = false, Inherited = true)]
	public sealed class FilterableAttribute : Attribute
	{
		bool filterable;

		public FilterableAttribute (bool filterable) 
		{
			this.filterable = filterable;
		}

		public static readonly FilterableAttribute Default = new FilterableAttribute (true);

		public static readonly FilterableAttribute No = new FilterableAttribute (false);

		public static readonly FilterableAttribute Yes = new FilterableAttribute (true);
		
		public bool Filterable { 
			get { return filterable; } 
		}

		public override bool Equals (object obj)
		{
			if (obj != null && obj is FilterableAttribute) {
				FilterableAttribute fa = (FilterableAttribute) obj;
				return (this.filterable == fa.filterable);
			}
			return false;
		}

		public override int GetHashCode ()
		{
			return this.filterable.GetHashCode ();
		}

		public override bool IsDefaultAttribute ()
		{
			return Equals (Default);
		}

		public static bool IsObjectFilterable (object obj)
		{
			return IsTypeFilterable (obj.GetType ());
		}

		public static bool IsPropertyFilterable (PropertyDescriptor propDesc)
		{
			System.ComponentModel.AttributeCollection attributes = propDesc.Attributes;
			if (attributes.Count != 0) {
				foreach (Attribute attrib in attributes)
					if (attrib is FilterableAttribute)
						return true;
			}
			return false;
		}

		public static bool IsTypeFilterable (Type type)
		{
			Object [] attributes = type.GetCustomAttributes (typeof (FilterableAttribute), false);
			if (attributes.Length != 0) {
				foreach (Attribute attrib in attributes)
					if (attrib is FilterableAttribute)
						return true;
			}
			return false;
		}
	}
}
