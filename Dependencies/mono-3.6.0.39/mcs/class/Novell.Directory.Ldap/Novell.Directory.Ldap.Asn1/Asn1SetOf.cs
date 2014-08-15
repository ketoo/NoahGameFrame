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
// Novell.Directory.Ldap.Asn1.Asn1SetOf.cs
//
// Author:
//   Sunil Kumar (Sunilk@novell.com)
//
// (C) 2003 Novell, Inc (http://www.novell.com)
//

using System;

namespace Novell.Directory.Ldap.Asn1
{
	
	/// <summary> The Asn1Set class can hold an unordered collection of components with
	/// identical type. This class inherits from the Asn1Structured class
	/// which already provides functionality to hold multiple Asn1 components.
	/// </summary>
	public class Asn1SetOf:Asn1Structured
	{
		
		/// <summary> ASN.1 SET OF tag definition.</summary>
		public const int TAG = 0x11;
		
		/// <summary> ID is added for Optimization.
		/// 
		/// Id needs only be one Value for every instance,
		/// thus we create it only once.
		/// </summary>
		public static readonly Asn1Identifier ID = new Asn1Identifier(Asn1Identifier.UNIVERSAL, true, TAG);
		/* Constructors for Asn1SetOf
		*/
		
		/// <summary> Constructs an Asn1SetOf object with no actual
		/// Asn1Objects in it. Assumes a default size of 5 elements.
		/// </summary>
		public Asn1SetOf():base(ID)
		{
			return ;
		}
		
		
		/// <summary> Constructs an Asn1SetOf object with the specified
		/// number of placeholders for Asn1Objects. However there
		/// are no actual Asn1Objects in this SequenceOf object.
		/// 
		/// </summary>
		/// <param name="size">Specifies the initial size of the collection.
		/// </param>
		public Asn1SetOf(int size):base(ID, size)
		{
			return ;
		}
		
		
		/// <summary> A copy constructor that creates an Asn1SetOf from an instance of Asn1Set.
		/// 
		/// Since SET and SET_OF have the same identifier, the decoder
		/// will always return a SET object when it detects that identifier.
		/// In order to take advantage of the Asn1SetOf type, we need to be
		/// able to construct this object when knowingly receiving an
		/// Asn1Set.
		/// </summary>
		public Asn1SetOf(Asn1Set set_Renamed):base(ID, set_Renamed.toArray(), set_Renamed.size())
		{
			return ;
		}
		
		/* Asn1SetOf specific methods
		*/
		
		/// <summary> Return a String representation of this Asn1SetOf.</summary>
		[CLSCompliantAttribute(false)]
		public override System.String ToString()
		{
			return base.toString("SET OF: { ");
		}
	}
}
