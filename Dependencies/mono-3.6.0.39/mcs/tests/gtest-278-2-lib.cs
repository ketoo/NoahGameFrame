// Compiler options: -t:library

// CECIL FIXME: Every method should actually throw new ApplicationException ("Should not be called");
// but because mcs dependency on System.Reflection it cannot be done

using System;

public class C
{
	public class CC
	{
		public static string Print ()
		{
			return typeof (CC).FullName;
		}
	}

	public static string Print ()
	{
		return typeof (C).FullName;
	}
}

public class D
{
	public static string Print ()
	{
		return typeof (D).FullName;
	}
}

public struct G<T>
{
	public static T Test (T t)
	{
		return t;
	}
}
