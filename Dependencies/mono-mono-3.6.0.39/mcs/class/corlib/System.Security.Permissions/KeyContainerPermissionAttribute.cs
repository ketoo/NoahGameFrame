//
// System.Security.Permissions.KeyContainerPermissionAttribute class
//
// Author
//	Sebastien Pouliot  <sebastien@ximian.com>
//
// Copyright (C) 2004-2005 Novell, Inc (http://www.novell.com)
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

using System.Globalization;
using System.Runtime.InteropServices;

namespace System.Security.Permissions {

	[AttributeUsage (AttributeTargets.Assembly | AttributeTargets.Class |
		AttributeTargets.Struct | AttributeTargets.Constructor | AttributeTargets.Method, 
		AllowMultiple = true, Inherited = false)]
	[ComVisible (true)]
	[Serializable]
	public sealed class KeyContainerPermissionAttribute : CodeAccessSecurityAttribute {

		private KeyContainerPermissionFlags _flags;
		private string _containerName;
		private int _spec;
		private string _store;
		private string _providerName;
		private int _type;

		// Constructors

		public KeyContainerPermissionAttribute (SecurityAction action) 
			: base (action)
		{
			_spec = -1;
			_type = -1;
		}

		// Properties

		public KeyContainerPermissionFlags Flags {
			get { return _flags; }
			set { _flags = value; }
		}

		public string KeyContainerName {
			get { return _containerName; }
			set { _containerName = value; }
		}

		public int KeySpec {
			get { return _spec; }
			set { _spec = value; }
		}

		public string KeyStore {
			get { return _store; }
			set { _store = value; }
		}

		public string ProviderName {
			get { return _providerName; }
			set { _providerName = value; }
		}

		public int ProviderType {
			get { return _type; }
			set { _type = value; }
		}

		// Methods

		public override IPermission CreatePermission ()
		{
			if (this.Unrestricted)
				return new KeyContainerPermission (PermissionState.Unrestricted);
			else if (EmptyEntry ())
				return new KeyContainerPermission (_flags);
			else {
				KeyContainerPermissionAccessEntry[] list = new KeyContainerPermissionAccessEntry [1];
				list [0] = new KeyContainerPermissionAccessEntry (_store, _providerName, _type, _containerName, _spec, _flags);
				return new KeyContainerPermission (_flags, list);
			}
		}

		private bool EmptyEntry () 
		{
			if (_containerName != null)
				return false;
			if (_spec != 0)
				return false;
			if (_store != null)
				return false;
			if (_providerName != null)
				return false;
			if (_type != 0)
				return false;
			return true;
		}
	}
}

