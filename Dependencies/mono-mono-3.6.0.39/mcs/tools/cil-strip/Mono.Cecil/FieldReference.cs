//
// FieldReference.cs
//
// Author:
//   Jb Evain (jbevain@gmail.com)
//
// (C) 2005 Jb Evain
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

namespace Mono.Cecil {

	using Mono.Cecil;

	internal class FieldReference : MemberReference {

		TypeReference m_fieldType;

		public TypeReference FieldType {
			get { return m_fieldType; }
			set { m_fieldType = value; }
		}

		internal FieldReference (string name, TypeReference fieldType) : base (name)
		{
			m_fieldType = fieldType;
		}

		public FieldReference (string name, TypeReference declaringType, TypeReference fieldType) :
			this (name, fieldType)
		{
			this.DeclaringType = declaringType;
		}

		public virtual FieldDefinition Resolve ()
		{
			TypeReference declaringType = DeclaringType;
			if (declaringType == null)
				return null;

			return declaringType.Module.Resolver.Resolve (this);
		}

		public override string ToString ()
		{
			return string.Concat (m_fieldType.FullName, " ", base.ToString ());
		}
	}
}
