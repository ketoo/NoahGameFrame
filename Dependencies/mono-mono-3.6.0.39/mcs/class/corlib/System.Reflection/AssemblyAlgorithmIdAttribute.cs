//
// System.Reflection.AssemblyAlgorithmIdAttribute.cs
//
// Author: Duncan Mak <duncan@ximian.com>
//
// (C) 2002 Ximian, Inc. http://www.ximian.com
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

using System;
using System.Configuration.Assemblies;
using System.Runtime.InteropServices;

namespace System.Reflection
{
	[ComVisible (true)]
	[AttributeUsage (AttributeTargets.Assembly, Inherited=false)]
	public sealed class AssemblyAlgorithmIdAttribute : Attribute
	{
		// Field
		private uint id;
		
		// Constructor
		public AssemblyAlgorithmIdAttribute (AssemblyHashAlgorithm algorithmId)
		{
			id = (uint) algorithmId;
		}
		
		[CLSCompliant (false)]
		public AssemblyAlgorithmIdAttribute (uint algorithmId)
		{
			id = algorithmId;
		}
		
		// Property
		[CLSCompliant (false)]
		public uint AlgorithmId
		{
			get { return id; }
		}
	}
}
