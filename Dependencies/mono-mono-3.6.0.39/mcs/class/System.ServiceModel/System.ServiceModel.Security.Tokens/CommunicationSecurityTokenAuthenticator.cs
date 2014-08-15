//
// CommunicationSecurityTokenAuthenticator.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2007 Novell, Inc.  http://www.novell.com
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

using System.Collections.ObjectModel;
using System.IdentityModel.Policy;
using System.IdentityModel.Selectors;
using System.IdentityModel.Tokens;

namespace System.ServiceModel.Security.Tokens
{
	abstract class CommunicationSecurityTokenAuthenticator
		: SecurityTokenAuthenticator, ICommunicationObject,
		  IIssuanceSecurityTokenAuthenticator
	{
		protected CommunicationSecurityTokenAuthenticator ()
		{
		}

		IssuedSecurityTokenHandler issuance_handler;
		RenewedSecurityTokenHandler renew_handler;

		public IssuedSecurityTokenHandler IssuedSecurityTokenHandler { 
			get { return issuance_handler; }
			set { issuance_handler = value; }
		}

		public RenewedSecurityTokenHandler RenewedSecurityTokenHandler { 
			get { return renew_handler; }
			set { renew_handler = value; }
		}

		public abstract AuthenticatorCommunicationObject Communication { get; }

		[MonoTODO]
		protected override bool CanValidateTokenCore (SecurityToken token)
		{
			throw new NotImplementedException ();
		}

		[MonoTODO]
		protected override ReadOnlyCollection<IAuthorizationPolicy> ValidateTokenCore (SecurityToken token)
		{
			throw new NotImplementedException ();
		}

		public CommunicationState State {
			get { return Communication.State; }
		}

		public void Abort ()
		{
			Communication.Abort ();
		}

		public void Open ()
		{
			Communication.Open ();
		}

		public void Open (TimeSpan timeout)
		{
			Communication.Open (timeout);
		}

		public IAsyncResult BeginOpen (AsyncCallback callback, object state)
		{
			return Communication.BeginOpen (callback, state);
		}

		public IAsyncResult BeginOpen (TimeSpan timeout, AsyncCallback callback, object state)
		{
			return Communication.BeginOpen (timeout, callback, state);
		}

		public void EndOpen (IAsyncResult result)
		{
			Communication.EndOpen (result);
		}

		public void Close ()
		{
			Communication.Close ();
		}

		public void Close (TimeSpan timeout)
		{
			Communication.Close (timeout);
		}

		public IAsyncResult BeginClose (AsyncCallback callback, object state)
		{
			return Communication.BeginClose (callback, state);
		}

		public IAsyncResult BeginClose (TimeSpan timeout, AsyncCallback callback, object state)
		{
			return Communication.BeginClose (timeout, callback, state);
		}

		public void EndClose (IAsyncResult result)
		{
			Communication.EndClose (result);
		}

		public event EventHandler Opening {
			add { Communication.Opening += value; }
			remove { Communication.Opening -= value; }
		}

		public event EventHandler Opened {
			add { Communication.Opened += value; }
			remove { Communication.Opened -= value; }
		}

		public event EventHandler Closing {
			add { Communication.Closing += value; }
			remove { Communication.Closing -= value; }
		}

		public event EventHandler Closed {
			add { Communication.Closed += value; }
			remove { Communication.Closed -= value; }
		}

		public event EventHandler Faulted {
			add { Communication.Faulted += value; }
			remove { Communication.Faulted -= value; }
		}
	}
}
