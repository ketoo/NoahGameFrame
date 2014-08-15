// Compiler options: -doc:xml-011.xml
using System;

namespace Testing
{
	public class Test
	{
		/// <summary>
		/// comment for public field
		/// </summary>
		public string PublicField;

		/// <summary>
		/// comment for public field
		/// </invalid>
		public string PublicField2;

		/**
		 <summary>
		 Javadoc comment for public field
		 </summary>
		*/
		public string PublicField3;

		public static void Main ()
		{
		}
	}
}

