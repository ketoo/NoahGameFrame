// CS0425: The constraints for type parameter `T' of method `CA.Foo<T>()' must match the constraints for type parameter `U' of interface method `IA.Foo<U>()'. Consider using an explicit interface implementation instead
// Line: 5
// Compiler options: -r:CS0425-7-lib.dll

public class CB : CA, IA
{
	public static void Main ()
	{
	}
}
