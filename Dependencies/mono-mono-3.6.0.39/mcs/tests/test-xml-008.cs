// Compiler options: -doc:xml-008.xml
using System;

namespace Testing
{
	/// comment without markup on class - it is allowed
	public class Test
	{
		public static void Main ()
		{
		}
	}

	/// <6roken> broken markup
	public class Test2
	{
	}

	/// <dont-forget-close-tag>
	public class Test3
	{
	}
}

