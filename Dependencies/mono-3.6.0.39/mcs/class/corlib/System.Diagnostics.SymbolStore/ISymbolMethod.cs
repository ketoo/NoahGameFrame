//
// System.Diagnostics.SymbolStore.ISymbolMethod
//
// Author:
//   Duco Fijma (duco@lorentz.xs4all.nl)
//
//   (c) 2002 Duco Fijma
// 

//
// Copyright (C) 2004 Novell, Inc (http://www.novell.com)
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

namespace System.Diagnostics.SymbolStore
{
	[ComVisible (true)]
	public interface ISymbolMethod {
		ISymbolScope RootScope {get; }
		int SequencePointCount {get; }
		SymbolToken Token {get; }

		ISymbolNamespace GetNamespace ();
		int GetOffset (ISymbolDocument document, int line, int column);
		ISymbolVariable[] GetParameters ();
		int[] GetRanges (ISymbolDocument document, int line, int column);
		ISymbolScope GetScope (int offset);
		void GetSequencePoints (
			int[] offsets,
			ISymbolDocument[] documents,
			int[] lines,
			int[] columns,
			int[] endLines,
			int[] endColumns);
		bool GetSourceStartEnd (
			ISymbolDocument[] docs,
			int[] lines,
			int[] columns); 
	}
}
