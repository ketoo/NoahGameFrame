// CS0815: An implicitly typed local variable declaration cannot be initialized with `void'
// Line: 9


public class Test
{
	static void Main ()
	{
		var v = Foo ();
	}

	static void Foo ()
	{
	}
}

