// CS1591: Missing XML comment for publicly visible type or member `Testing.Test.operator !(Testing.Test)'
// Line: 14
// Compiler options: -doc:dummy.xml -warnaserror -warn:4

using System;

namespace Testing
{
	/// <summary>
	/// description for class Test
	/// </summary>
	public class Test
	{
		public static bool operator ! (Test t)
		{
			return false;
		}
	}
}
