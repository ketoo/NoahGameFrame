// CS0111: A member `C.Foo()' is already defined. Rename this member or use different parameter types
// Line: 11


public partial class C
{
	void Foo ()
	{
	}
	
	partial void Foo ();
	
	partial void Foo ()
	{
	}
}
