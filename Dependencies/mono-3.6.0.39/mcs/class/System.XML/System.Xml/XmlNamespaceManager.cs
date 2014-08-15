//
// XmlNamespaceManager.cs
//
// Authors:
//   Jason Diamond (jason@injektilo.org)
//   Ben Maurer (bmaurer@users.sourceforge.net)
//   Atsushi Enomoto (atsushi@ximian.com)
//
// (C) 2001 Jason Diamond  http://injektilo.org/
// (C) 2003 Ben Maurer
// (C) 2004 Novell Inc.
//

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

using System.Collections;
#if NET_2_0
using System.Collections.Generic;
#endif
using System.Collections.Specialized;

namespace System.Xml
{
	public class XmlNamespaceManager : IXmlNamespaceResolver, IEnumerable
	{
		#region Data
		struct NsDecl {
			public string Prefix, Uri;
		}
		
		struct NsScope {
			public int DeclCount;
			public string DefaultNamespace;
		}
		
		NsDecl [] decls;
		int declPos = -1;
		
		NsScope [] scopes;
		int scopePos = -1;
		
		string defaultNamespace;
		int count;
		
		void InitData ()
		{
			decls = new NsDecl [10];
			scopes = new NsScope [40];
		}
		
		// precondition declPos == nsDecl.Length
		void GrowDecls ()
		{
			NsDecl [] old = decls;
			decls = new NsDecl [declPos * 2 + 1];
			if (declPos > 0)
				Array.Copy (old, 0, decls, 0, declPos);
		}
		
		// precondition scopePos == scopes.Length
		void GrowScopes ()
		{
			NsScope [] old = scopes;
			scopes = new NsScope [scopePos * 2 + 1];
			if (scopePos > 0)
				Array.Copy (old, 0, scopes, 0, scopePos);
		}
		
		#endregion
		
		#region Fields

		private XmlNameTable nameTable;
		internal const string XmlnsXml = "http://www.w3.org/XML/1998/namespace";
		internal const string XmlnsXmlns = "http://www.w3.org/2000/xmlns/";
		internal const string PrefixXml = "xml";
		internal const string PrefixXmlns = "xmlns";
		internal bool internalAtomizedNames;

		#endregion

		#region Constructor

		public XmlNamespaceManager (XmlNameTable nameTable)
		{
			if (nameTable == null)
				throw new ArgumentNullException ("nameTable");
			this.nameTable = nameTable;

			nameTable.Add (PrefixXmlns);
			nameTable.Add (PrefixXml);
			nameTable.Add (String.Empty);
			nameTable.Add (XmlnsXmlns);
			nameTable.Add (XmlnsXml);
			
			InitData ();
		}

		#endregion

		#region Properties

		public virtual string DefaultNamespace {
			get { return defaultNamespace == null ? string.Empty : defaultNamespace; }
		}

#if NET_2_0
		public virtual XmlNameTable NameTable {
#else
		public XmlNameTable NameTable {
#endif
			get { return nameTable; }
		}

		#endregion

		#region Methods

		public virtual void AddNamespace (string prefix, string uri)
		{
			AddNamespace (prefix, uri, false);
		}

		void AddNamespace (string prefix, string uri, bool atomizedNames)
		{
			if (prefix == null)
				throw new ArgumentNullException ("prefix", "Value cannot be null.");

			if (uri == null)
				throw new ArgumentNullException ("uri", "Value cannot be null.");
			if (!atomizedNames) {
				prefix = nameTable.Add (prefix);
				uri = nameTable.Add (uri);
			}

			if (prefix == PrefixXml && uri == XmlnsXml)
				return;

			IsValidDeclaration (prefix, uri, true);

			if (prefix.Length == 0)
				defaultNamespace = uri;
			
			for (int i = declPos; i > declPos - count; i--) {
				if (object.ReferenceEquals (decls [i].Prefix, prefix)) {
					decls [i].Uri = uri;
					return;
				}
			}
			
			declPos ++;
			count ++;
			
			if (declPos == decls.Length)
				GrowDecls ();
			decls [declPos].Prefix = prefix;
			decls [declPos].Uri = uri;
		}

		static string IsValidDeclaration (string prefix, string uri, bool throwException)
		{
			string message = null;
			// It is funky, but it does not check whether prefix
			// is equivalent to "xml" in case-insensitive means.
			if (prefix == PrefixXml && uri != XmlnsXml)
				message = String.Format ("Prefix \"xml\" can only be bound to the fixed namespace URI \"{0}\". \"{1}\" is invalid.", XmlnsXml, uri);
			else if (message == null && prefix == "xmlns")
				message = "Declaring prefix named \"xmlns\" is not allowed to any namespace.";
			else if (message == null && uri == XmlnsXmlns)
				message = String.Format ("Namespace URI \"{0}\" cannot be declared with any namespace.", XmlnsXmlns);
			if (message != null && throwException)
				throw new ArgumentException (message);
			else
				return message;
		}

		public virtual IEnumerator GetEnumerator ()
		{
			// In fact it returns such table's enumerator that contains all the namespaces.
			// while HasNamespace() ignores pushed namespaces.
			
			Hashtable ht = new Hashtable ();
			for (int i = 0; i <= declPos; i++) {
				if (decls [i].Prefix != string.Empty && decls [i].Uri != null) {
					ht [decls [i].Prefix] = decls [i].Uri;
				}
			}
			
			ht [string.Empty] = DefaultNamespace;
			ht [PrefixXml] = XmlnsXml;
			ht [PrefixXmlns] = XmlnsXmlns;
			
			return ht.Keys.GetEnumerator ();
		}

#if NET_2_0
		public virtual IDictionary<string, string> GetNamespacesInScope (XmlNamespaceScope scope)
		{
			IDictionary namespaceTable = GetNamespacesInScopeImpl (scope);
			IDictionary<string, string> namespaces = new Dictionary<string, string>(namespaceTable.Count);

			foreach (DictionaryEntry entry in namespaceTable) {
				namespaces[(string) entry.Key] = (string) entry.Value;
			}
			return namespaces;
		}
#else
		IDictionary IXmlNamespaceResolver.GetNamespacesInScope (XmlNamespaceScope scope)
		{
			return GetNamespacesInScopeImpl (scope);
		}
#endif

		internal virtual IDictionary GetNamespacesInScopeImpl (XmlNamespaceScope scope)
		{
			Hashtable table = new Hashtable ();

			if (scope == XmlNamespaceScope.Local) {
				for (int i = 0; i < count; i++)
					if (decls [declPos - i].Prefix == String.Empty && decls [declPos - i].Uri == String.Empty) {
						if (table.Contains (String.Empty))
							table.Remove (String.Empty);
					}
					else if (decls [declPos - i].Uri != null)
						table.Add (decls [declPos - i].Prefix, decls [declPos - i].Uri);
				return table;
			} else {
				for (int i = 0; i <= declPos; i++) {
					if (decls [i].Prefix == String.Empty && decls [i].Uri == String.Empty) {
						// removal of default namespace
						if (table.Contains (String.Empty))
							table.Remove (String.Empty);
					}
					else if (decls [i].Uri != null)
						table [decls [i].Prefix] = decls [i].Uri;
				}

				if (scope == XmlNamespaceScope.All)
					table.Add ("xml", XmlNamespaceManager.XmlnsXml);
				return table;
			}
		}

		public virtual bool HasNamespace (string prefix)
		{
			return HasNamespace (prefix, false);
		}

		bool HasNamespace (string prefix, bool atomizedNames)
		{
			if (prefix == null || count == 0)
				return false;

			for (int i = declPos; i > declPos - count; i--) {
				if (decls [i].Prefix == prefix)
					return true;
			}
			
			return false;
		}

		public virtual string LookupNamespace (string prefix)
		{
			switch (prefix) {
			case PrefixXmlns:
				return nameTable.Get (XmlnsXmlns);
			case PrefixXml:
				return nameTable.Get (XmlnsXml);
			case "":
				return DefaultNamespace;
			case null:
				return null;
			}

			for (int i = declPos; i >= 0; i--) {
				if (CompareString (decls [i].Prefix, prefix, internalAtomizedNames) && decls [i].Uri != null /* null == flag for removed */)
					return decls [i].Uri;
			}
			
			return null;
		}

		internal string LookupNamespace (string prefix, bool atomizedNames)
		{
			internalAtomizedNames = atomizedNames;
			string ret = LookupNamespace (prefix);
			internalAtomizedNames = false;
			return ret;

		}

		public virtual string LookupPrefix (string uri)
		{
#if NET_2_0
			return LookupPrefix (uri, false);
#else
			return LookupPrefix (uri, true);
#endif
		}

		private bool CompareString (string s1, string s2, bool atomizedNames)
		{
			if (atomizedNames)
				return object.ReferenceEquals (s1, s2);
			else
				return s1 == s2;
		}

		internal string LookupPrefix (string uri, bool atomizedName)
		{
			return LookupPrefixCore (uri, atomizedName, false);
		}

		internal string LookupPrefixExclusive (string uri, bool atomizedName)
		{
			return LookupPrefixCore (uri, atomizedName, true);
		}

		string LookupPrefixCore (string uri, bool atomizedName, bool excludeOverriden)
		{
			if (uri == null)
				return null;

			if (CompareString (uri, DefaultNamespace, atomizedName))
				return string.Empty;

			if (CompareString (uri, XmlnsXml, atomizedName))
				return PrefixXml;
			
			if (CompareString (uri, XmlnsXmlns, atomizedName))
				return PrefixXmlns;

			for (int i = declPos; i >= 0; i--) {
				if (CompareString (decls [i].Uri, uri, atomizedName) && decls [i].Prefix.Length > 0) // we already looked for ""
					if (!excludeOverriden || !IsOverriden (i))
						return decls [i].Prefix;
			}

			// ECMA specifies that this method returns String.Empty
			// in case of no match. But actually MS.NET returns null.
			// For more information,see
			//  http://lists.ximian.com/archives/public/mono-list/2003-January/005071.html
			//return String.Empty;
			return null;
		}

		bool IsOverriden (int idx)
		{
			if (idx == declPos)
				return false;
			string prefix = decls [idx + 1].Prefix;
			for (int i = idx + 1; i <= declPos; i++)
				if ((object) decls [idx].Prefix == (object) prefix)
					return true;
			return false;
		}

		public virtual bool PopScope ()
		{
			if (scopePos == -1)
				return false;

			declPos -= count;
			defaultNamespace = scopes [scopePos].DefaultNamespace;
			count = scopes [scopePos].DeclCount;
			scopePos --;
			return true;
		}

		public virtual void PushScope ()
		{
			scopePos ++;
			if (scopePos == scopes.Length)
				GrowScopes ();
			
			scopes [scopePos].DefaultNamespace = defaultNamespace;
			scopes [scopePos].DeclCount = count;
			count = 0;
		}

		// It is rarely used, so we don't need NameTable optimization on it.
		public virtual void RemoveNamespace (string prefix, string uri)
		{
			RemoveNamespace (prefix, uri, false);
		}

		void RemoveNamespace (string prefix, string uri, bool atomizedNames)
		{
			if (prefix == null)
				throw new ArgumentNullException ("prefix");

			if (uri == null)
				throw new ArgumentNullException ("uri");
			
			if (count == 0)
				return;

			for (int i = declPos; i > declPos - count; i--) {
				if (CompareString (decls [i].Prefix, prefix, atomizedNames) && CompareString (decls [i].Uri, uri, atomizedNames))
					decls [i].Uri = null;
			}
		}

		#endregion
	}
}
