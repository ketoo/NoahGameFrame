//
// Mono.Security.Protocol.Ntlm.Type3Message - Authentication
//
// Author:
//	Sebastien Pouliot <sebastien@ximian.com>
//	Atsushi Enomoto <atsushi@ximian.com>
//
// (C) 2003 Motus Technologies Inc. (http://www.motus.com)
// Copyright (C) 2004, 2007 Novell, Inc (http://www.novell.com)
//
// References
// a.	NTLM Authentication Scheme for HTTP, Ronald Tschalär
//	http://www.innovation.ch/java/ntlm.html
// b.	The NTLM Authentication Protocol, Copyright © 2003 Eric Glass
//	http://davenport.sourceforge.net/ntlm.html
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
using System.Globalization;
using System.Security.Cryptography;
using System.Text;

namespace Mono.Security.Protocol.Ntlm {

	public class Type3Message : MessageBase {

		private byte[] _challenge;
		private string _host;
		private string _domain;
		private string _username;
		private string _password;
		private byte[] _lm;
		private byte[] _nt;
		private byte[] _nonce;

		public Type3Message () : this (NtlmVersion.Version1)
		{
		}

		public Type3Message (NtlmVersion version) : base (3, version)
		{
			// default values
			_domain = Environment.UserDomainName;
			_host = Environment.MachineName;
			_username = Environment.UserName;
			Flags = (NtlmFlags) 0x8201;
			_nonce = new byte [8];
			RandomNumberGenerator rng = RandomNumberGenerator.Create ();
			rng.GetBytes (_nonce);
		}

		public Type3Message (byte[] message) : this (message, NtlmVersion.Version1)
		{
		}

		public Type3Message (byte[] message, NtlmVersion version) : base (3, version)
		{
			Decode (message);
		}

		~Type3Message () 
		{
			if (_challenge != null)
				Array.Clear (_challenge, 0, _challenge.Length);
			if (_lm != null)
				Array.Clear (_lm, 0, _lm.Length);
			if (_nt != null)
				Array.Clear (_nt, 0, _nt.Length);
		}

		// properties

		public byte[] Challenge {
			get { 
				if (_challenge == null)
					return null;
				return (byte[]) _challenge.Clone (); }
			set { 
				if (value == null)
					throw new ArgumentNullException ("Challenge");
				if (value.Length != 8) {
					string msg = Locale.GetText ("Invalid Challenge Length (should be 8 bytes).");
					throw new ArgumentException (msg, "Challenge");
				}
				_challenge = (byte[]) value.Clone (); 
			}
		}

		public string Domain {
			get { return _domain; }
			set { _domain = value; }
		}

		public string Host {
			get { return _host; }
			set { _host = value; }
		}

		public string Password {
			get { return _password; }
			set { _password = value; }
		}

		public string Username {
			get { return _username; }
			set { _username = value; }
		}

		public byte[] LM {
			get { return _lm; }
		}

		public byte[] NT {
			get { return _nt; }
		}

		// methods

		protected override void Decode (byte[] message) 
		{
			base.Decode (message);

			// FIXME: This Version condition is introduced to make
			// nunit tests pass, and hence not based on the NTLM 
			// analysis docs. Find out the reason why it is needed.
			if (Version == NtlmVersion.Version1 &&
			    BitConverterLE.ToUInt16 (message, 56) != message.Length) {
				string msg = Locale.GetText ("Invalid Type3 message length.");
				throw new ArgumentException (msg, "message");
			}

			_password = null;

			int dom_len = BitConverterLE.ToUInt16 (message, 28);
			int dom_off = BitConverterLE.ToUInt16 (message, 32);
			_domain = Encoding.Unicode.GetString (message, dom_off, dom_len);

			int host_len = BitConverterLE.ToUInt16 (message, 44);
			int host_off = BitConverterLE.ToUInt16 (message, 48);
			_host = Encoding.Unicode.GetString (message, host_off, host_len);

			int user_len = BitConverterLE.ToUInt16 (message, 36);
			int user_off = BitConverterLE.ToUInt16 (message, 40);
			_username = Encoding.Unicode.GetString (message, user_off, user_len);

			_lm = new byte [24];
			int lm_off = BitConverterLE.ToUInt16 (message, 16);
			Buffer.BlockCopy (message, lm_off, _lm, 0, 24);
			
			_nt = new byte [24];
			int nt_off = BitConverterLE.ToUInt16 (message, 24);
			Buffer.BlockCopy (message, nt_off, _nt, 0, 24);

			if (message.Length >= 64)
				Flags = (NtlmFlags) BitConverterLE.ToUInt32 (message, 60);
		}

		public override byte[] GetBytes () 
		{
			byte[] domain = Encoding.Unicode.GetBytes (_domain.ToUpper (CultureInfo.InvariantCulture));
			byte[] user = Encoding.Unicode.GetBytes (_username);
			byte[] host = Encoding.Unicode.GetBytes (_host.ToUpper (CultureInfo.InvariantCulture));

			int fixed_size = Version == NtlmVersion.Version3 ? 72 : 64;
			short skey_len = (short) (Version != NtlmVersion.Version1 ? 16 : 0);
			int skey_off = 0;

			byte[] data = PrepareMessage (fixed_size + domain.Length + user.Length + host.Length + 24 + 24 + skey_len);

			// LM response
			int lmresp_off = fixed_size + domain.Length + user.Length + host.Length;
			data [12] = (byte) 0x18;
			data [13] = (byte) 0x00;
			data [14] = (byte) 0x18;
			data [15] = (byte) 0x00;
			data [16] = (byte) lmresp_off;
			data [17] = (byte)(lmresp_off >> 8);
			data [18] = (byte)(lmresp_off >> 16);
			data [19] = (byte)(lmresp_off >> 24);

			// NT response
			int ntresp_off = lmresp_off + 24;
			data [20] = (byte) 0x18;
			data [21] = (byte) 0x00;
			data [22] = (byte) 0x18;
			data [23] = (byte) 0x00;
			data [24] = (byte) ntresp_off;
			data [25] = (byte)(ntresp_off >> 8);
			data [26] = (byte)(ntresp_off >> 16);
			data [27] = (byte)(ntresp_off >> 24);

			// domain
			short dom_len = (short)domain.Length;
			int dom_off = (short)fixed_size;
			data [28] = (byte) dom_len;
			data [29] = (byte)(dom_len >> 8);
			data [30] = data [28];
			data [31] = data [29];
			data [32] = (byte) dom_off;
			data [33] = (byte)(dom_off >> 8);
			data [34] = (byte)(dom_off >> 16);
			data [35] = (byte)(dom_off >> 24);

			// username
			short uname_len = (short)user.Length;
			int uname_off = dom_off + dom_len;
			data [36] = (byte) uname_len;
			data [37] = (byte)(uname_len >> 8);
			data [38] = data [36];
			data [39] = data [37];
			data [40] = (byte) uname_off;
			data [41] = (byte)(uname_off >> 8);
			data [42] = (byte)(uname_off >> 16);
			data [43] = (byte)(uname_off >> 24);

			// host
			short host_len = (short)host.Length;
			int host_off = uname_off + uname_len;
			data [44] = (byte) host_len;
			data [45] = (byte)(host_len >> 8);
			data [46] = data [44];
			data [47] = data [45];
			data [48] = (byte) host_off;
			data [49] = (byte)(host_off >> 8);
			data [50] = (byte)(host_off >> 16);
			data [51] = (byte)(host_off >> 24);

			// session key
			// FIXME: This Version condition is introduced to make
			// nunit tests pass, and hence not based on the NTLM 
			// analysis docs. Find out the reason why it is needed.
			if (Version != NtlmVersion.Version1) {
				skey_off = (short)(data.Length - skey_len);
				data [52] = (byte) skey_len;
				data [53] = (byte)(skey_len >> 8);
				data [54] = data [52];
				data [55] = data [53];
				data [56] = (byte) skey_off;
				data [57] = (byte)(skey_off >> 8);
				data [58] = (byte)(skey_off >> 16);
				data [59] = (byte)(skey_off >> 24);
			} else {
				// message length
				short msg_len = (short)data.Length;
				data [56] = (byte) msg_len;
				data [57] = (byte)(msg_len >> 8);
			}

			// options flags
			data [60] = (byte) Flags;
			data [61] = (byte)((uint)Flags >> 8);
			data [62] = (byte)((uint)Flags >> 16);
			data [63] = (byte)((uint)Flags >> 24);

			// osversion
			if (Version == NtlmVersion.Version3)
				Buffer.BlockCopy (OSVersion, 0, data, 64, OSVersion.Length);

			Buffer.BlockCopy (domain, 0, data, dom_off, domain.Length);
			Buffer.BlockCopy (user, 0, data, uname_off, user.Length);
			Buffer.BlockCopy (host, 0, data, host_off, host.Length);

			using (ChallengeResponse ntlm = new ChallengeResponse (_password, _challenge)) {
				Buffer.BlockCopy (ntlm.LM, 0, data, lmresp_off, 24);
				Buffer.BlockCopy (ntlm.NT, 0, data, ntresp_off, 24);

				if (Version != NtlmVersion.Version1)
					// session key
					Buffer.BlockCopy (ntlm.LMSessionKey, 0, data, skey_off, 16);
			}

			return data;
		}
	}
}
