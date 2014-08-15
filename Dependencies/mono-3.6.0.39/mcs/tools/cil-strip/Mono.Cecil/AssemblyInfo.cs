//
// AssemblyInfo.cs
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

using System;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

#if !EMBEDDED

[assembly: AssemblyTitle ("Mono.Cecil")]
[assembly: AssemblyDescription ("Library for reading and writing CIL images")]
[assembly: AssemblyConfiguration ("")]
[assembly: AssemblyProduct ("Mono.Cecil")]
[assembly: AssemblyCopyright ("(C) 2005 - 2007, Jb Evain")]
[assembly: AssemblyCulture ("")]

[assembly: CLSCompliant (false)]
[assembly: ComVisible (false)]

[assembly: AssemblyVersion (
	//
	// DO NOT MODIFY THE STRING BELOWS WITOUT UPDATING cecil.pc.in in mono/data
	//
	"0.6.9.0"
	//
	// DO NOT MODIFY THE STRING ABOVE WITHOUT UPDATING cecil.pc.in in mono/data
)]

#if KEYFILE
[assembly: AssemblyKeyFile("../../mono.snk")]
#endif

#endif



