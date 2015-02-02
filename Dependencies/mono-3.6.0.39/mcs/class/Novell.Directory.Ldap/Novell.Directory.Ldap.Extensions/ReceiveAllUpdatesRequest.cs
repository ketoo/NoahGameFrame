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
// Novell.Directory.Ldap.Extensions.ReceiveAllUpdatesRequest.cs
//
// Author:
//   Sunil Kumar (Sunilk@novell.com)
//
// (C) 2003 Novell, Inc (http://www.novell.com)
//

using System;
using Novell.Directory.Ldap;
using Novell.Directory.Ldap.Asn1;
using Novell.Directory.Ldap.Utilclass;

namespace Novell.Directory.Ldap.Extensions
{
	
	/// <summary>  Schedules a specified directory server to receive updates from another
	/// directory server for a specific replica.
	/// 
	/// The receiveAllUpdatesRequest extension uses the following OID:
	/// 2.16.840.1.113719.1.27.100.21
	/// 
	/// The requestValue has the following format:
	/// 
	/// requestValue ::=
	///    partitionRoot    LdapDN
	///    toServerDN       LdapDN
	///    fromServerDN     LdapDN
	/// </summary>
	public class ReceiveAllUpdatesRequest:LdapExtendedOperation
	{
		
		/// <summary> 
		/// Constructs an extended operation object for receiving all updates from
		/// another directory server for a specific replica.
		/// 
		/// </summary>
		/// <param name="partitionRoot">  The distinguished name of the replica
		/// that will be updated.
		/// 
		/// </param>
		/// <param name="toServerDN">     The distinguished name of the server holding the
		/// replica to be updated.
		/// 
		/// </param>
		/// <param name="fromServerDN">   The distinguished name of the server from which
		/// updates are sent.
		/// 
		/// </param>
		/// <exception> LdapException A general exception which includes an error message
		/// and an Ldap error code.
		/// </exception>
		public ReceiveAllUpdatesRequest(System.String partitionRoot, System.String toServerDN, System.String fromServerDN):base(ReplicationConstants.RECEIVE_ALL_UPDATES_REQ, null)
		{
			
			try
			{
				
				if (((System.Object) partitionRoot == null) || ((System.Object) toServerDN == null) || ((System.Object) fromServerDN == null))
					throw new System.ArgumentException(ExceptionMessages.PARAM_ERROR);
				
				System.IO.MemoryStream encodedData = new System.IO.MemoryStream();
				LBEREncoder encoder = new LBEREncoder();
				
				Asn1OctetString asn1_partitionRoot = new Asn1OctetString(partitionRoot);
				Asn1OctetString asn1_toServerDN = new Asn1OctetString(toServerDN);
				Asn1OctetString asn1_fromServerDN = new Asn1OctetString(fromServerDN);
				
				asn1_partitionRoot.encode(encoder, encodedData);
				asn1_toServerDN.encode(encoder, encodedData);
				asn1_fromServerDN.encode(encoder, encodedData);
				
				setValue(SupportClass.ToSByteArray(encodedData.ToArray()));
			}
			catch (System.IO.IOException ioe)
			{
				throw new LdapException(ExceptionMessages.ENCODING_ERROR, LdapException.ENCODING_ERROR, (System.String) null);
			}
		}
	}
}
