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
// Novell.Directory.Ldap.Extensions.GetBindDNRequest.cs
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
	
	/// <summary>  Returns the distingusihed name of the object your are logged in as.
	/// 
	/// To use this class, you must create an instance of the
	/// class and then call the extendedOperation method with this
	/// object as the required LdapExtendedOperation parameter.
	/// 
	/// The returned LdapExtendedResponse object can then be converted to
	/// a GetBindDNResponse object with the ExtendedREsponseFactory
	/// class. This object contains  methods for retrieving the distinguished
	/// name.
	/// 
	/// The GetBindDNRequest extension uses the following OID:
	/// 2.16.840.1.113719.1.27.100.31
	/// 
	/// The request value has a value of null.
	/// 
	/// </summary>
	public class GetBindDNRequest:LdapExtendedOperation
	{

		static GetBindDNRequest() 
		{
			/*
				* Register the extendedresponse class which is returned by the
				* server in response to a ListReplicasRequest
				*/
			try
			{
				LdapExtendedResponse.register(ReplicationConstants.GET_IDENTITY_NAME_RES, System.Type.GetType("Novell.Directory.Ldap.Extensions.GetBindDNResponse"));
			}
			catch (System.Exception e)
			{
				System.Console.Error.WriteLine("Could not register Extended Response -" + " Class not found");
			}
		}
		
		/// <summary>   Constructs an extended operation object for retrieving the bind dn.
		/// 
		/// </summary>
		/// <exception> LdapException A general exception which includes an error 
		/// message and an Ldap error code.    
		/// </exception>
		
		public GetBindDNRequest():base(ReplicationConstants.GET_IDENTITY_NAME_REQ, null)
		{
		}
	}
}
