//
// System.CodeDom CodeTypeReferenceExpression Class implementation
//
// Author:
//   Miguel de Icaza (miguel@ximian.com)
//   Daniel Stodden (stodden@in.tum.de)
//
// (C) 2001 Ximian, Inc.
// Copyright (C) 2005 Novell, Inc (http://www.novell.com)
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

using System.Runtime.InteropServices;

namespace System.CodeDom
{
	[Serializable]
	[ClassInterface(ClassInterfaceType.AutoDispatch)]
	[ComVisible(true)]
	public class CodeTypeReferenceExpression
		: CodeExpression
	{
		private CodeTypeReference type;
		
		//
		// Constructors
		//
		public CodeTypeReferenceExpression ()
		{
		}

		public CodeTypeReferenceExpression (CodeTypeReference type)
		{
			this.type = type;
		}
		
		public CodeTypeReferenceExpression (string type)
		{
			this.type = new CodeTypeReference( type );
		}

		public CodeTypeReferenceExpression (Type type)
		{
			this.type = new CodeTypeReference( type );
		}

		//
		// Properties
		//
		public CodeTypeReference Type {
			get {
				if (type == null) {
					return new CodeTypeReference (string.Empty);
				}
				return type;
			}
			set {
				type = value;
			}
		}

		//
		// ICodeDomVisitor method
		//
		internal override void Accept (ICodeDomVisitor visitor)
		{
			visitor.Visit (this);
		}
	}
}
