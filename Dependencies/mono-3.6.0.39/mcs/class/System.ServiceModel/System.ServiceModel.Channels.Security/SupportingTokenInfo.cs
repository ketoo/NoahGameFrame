//
// SupportingTokenInfo.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2005-2007 Novell, Inc.  http://www.novell.com
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
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IdentityModel.Selectors;
using System.IdentityModel.Tokens;
using System.Net.Security;
using System.Security.Cryptography.Xml;
using System.ServiceModel.Channels;
using System.ServiceModel.Description;
using System.ServiceModel.Security;
using System.ServiceModel.Security.Tokens;

using ReqType = System.ServiceModel.Security.Tokens.ServiceModelSecurityTokenRequirement;

namespace System.ServiceModel.Channels.Security
{
	internal class SupportingTokenInfo
	{
		public SupportingTokenInfo (SecurityToken token,
			SecurityTokenAttachmentMode mode,
			bool isOptional)
		{
			Token = token;
			Mode = mode;
			IsOptional = isOptional;
		}

		public SecurityToken Token;
		public SecurityTokenAttachmentMode Mode;
		public bool IsOptional;
		public EncryptedData Encrypted;
	}

	internal class SupportingTokenInfoCollection : Collection<SupportingTokenInfo>
	{
		protected override void InsertItem (int index, SupportingTokenInfo item)
		{
			foreach (SupportingTokenInfo i in this)
				if (i.Token.GetType () == item.Token.GetType ())
					throw new ArgumentException (String.Format ("Supporting tokens do not allow multiple SecurityTokens of the same type: {0}", i.Token.GetType ()));
			base.InsertItem (index, item);
		}
	}
}
