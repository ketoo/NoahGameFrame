/******************************************************************************
* The MIT License
* Copyright (c) 2003 Novell Inc.  www.novell.com
* 
* Permission is hereby granted, free of charge, to any person obtaining  a copy
* of this software and associated documentation files (the Software), to deal
* in the Software without restriction, including  without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
* copies of the Software, and to  permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in 
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*******************************************************************************/
//
// Novell.Directory.Ldap.Rfc2251.RfcSubstringFilter.cs
//
// Author:
//   Sunil Kumar (Sunilk@novell.com)
//
// (C) 2003 Novell, Inc (http://www.novell.com)
//

using System;
using Novell.Directory.Ldap.Asn1;

namespace Novell.Directory.Ldap.Rfc2251
{
	
	/// <summary> Represents an Ldap Substring Filter.
	/// 
	/// <pre>
	/// SubstringFilter ::= SEQUENCE {
	/// type            AttributeDescription,
	/// -- at least one must be present
	/// substrings      SEQUENCE OF CHOICE {
	/// initial [0] LdapString,
	/// any     [1] LdapString,
	/// final   [2] LdapString } }
	/// </pre>
	/// </summary>
	public class RfcSubstringFilter:Asn1Sequence
	{
		
		//*************************************************************************
		// Constructors for SubstringFilter
		//*************************************************************************
		
		/// <summary> </summary>
		public RfcSubstringFilter(RfcAttributeDescription type, Asn1SequenceOf substrings):base(2)
		{
			add(type);
			add(substrings);
		}
	}
}
