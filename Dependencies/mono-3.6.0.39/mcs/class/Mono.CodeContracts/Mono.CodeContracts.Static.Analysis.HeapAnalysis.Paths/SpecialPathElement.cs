// 
// SpecialPathElement.cs
// 
// Authors:
//	Alexander Chebaturkin (chebaturkin@gmail.com)
// 
// Copyright (C) 2011 Alexander Chebaturkin
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
using Mono.CodeContracts.Static.AST;
using Mono.CodeContracts.Static.DataStructures;
using Mono.CodeContracts.Static.Providers;

namespace Mono.CodeContracts.Static.Analysis.HeapAnalysis.Paths {
	class SpecialPathElement : PathElement<SpecialPathElementKind> {
		private object type;

		public SpecialPathElement (SpecialPathElementKind element, SymFunction c) : base (element, element.ToString (), c)
		{
			this.castTo = "";
		}

		public override bool IsAddressOf
		{
			get { return false; }
		}

		public override bool IsDeref
		{
			get { return this.Element == SpecialPathElementKind.Deref; }
		}


		public override bool TryGetResultType (out TypeNode resultType)
		{
			if (this.type is TypeNode) {
				resultType = (TypeNode) this.type;
				return true;
			}

			resultType = default (TypeNode);
			return false;
		}

		public override bool TrySetType (TypeNode expectedType, IMetaDataProvider metaDataProvider, out TypeNode resultType)
		{
			switch (this.Element) {
			case SpecialPathElementKind.Length:
				this.castTo = metaDataProvider.IsArray (expectedType)
				              || metaDataProvider.System_String.Equals (expectedType)
				              	? ""
				              	: "System.Array";
				resultType = metaDataProvider.System_Int32;
				return true;
			case SpecialPathElementKind.Deref:
				if (metaDataProvider.IsManagedPointer (expectedType)) {
					TypeNode type = metaDataProvider.ElementType (expectedType);
					this.type = type;
					resultType = type;
					return true;
				}
				resultType = default(TypeNode);
				return false;

			default:
				resultType = default(TypeNode);
				return false;
			}
		}

		public override Result Decode<Data, Result, Visitor, Label> (Label label, Visitor visitor, Data data)
		{
			switch (this.Element) {
			case SpecialPathElementKind.Length:
				return visitor.LoadLength (label, Dummy.Value, Dummy.Value, data);
			case SpecialPathElementKind.Deref:
				throw new NotImplementedException ();
			default:
				throw new InvalidOperationException ();
			}
		}
	}
}
