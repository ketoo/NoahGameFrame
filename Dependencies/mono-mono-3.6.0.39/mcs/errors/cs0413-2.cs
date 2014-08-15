// CS0413: The `as' operator cannot be used with a non-reference type parameter `T'. Consider adding `class' or a reference type constraint
// Line: 8

public class SomeClass {
}

public class Foo<T> where T : struct {
	public T Do (object o) { return o as T; }
}

class Driver {
	static void Main ()
	{
		Foo<SomeClass> f = new Foo<SomeClass> ();
		f.Do ("something");
	}
}

