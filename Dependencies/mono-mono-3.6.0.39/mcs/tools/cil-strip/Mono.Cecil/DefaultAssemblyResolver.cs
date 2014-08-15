//
// DefaultAssemblyResolver.cs
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

namespace Mono.Cecil {

	using System.Collections;

	internal class DefaultAssemblyResolver : BaseAssemblyResolver {

		IDictionary m_cache;

		public DefaultAssemblyResolver ()
		{
			m_cache = new Hashtable ();
		}

		public override AssemblyDefinition Resolve (AssemblyNameReference name)
		{
			AssemblyDefinition asm = (AssemblyDefinition) m_cache [name.FullName];
			if (asm == null) {
				asm = base.Resolve (name);
				m_cache [name.FullName] = asm;
			}

			return asm;
		}

		protected void RegisterAssembly (AssemblyDefinition assembly)
		{
			string key = assembly.Name.FullName;
			if (m_cache.Contains (key))
				return;

			m_cache [key] = assembly;
		}
	}
}
