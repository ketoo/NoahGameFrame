// CS1612: Cannot modify a value type return value of `Test.v()'. Consider storing the value in a temporary variable
// Line: 28

public struct V
{
	public int this [int i] {
		set {
		}
	}
	
	public int x;
}

class Test
{
	V m_value;

	public V v() { return m_value; }

	public Test ()
	{
		m_value = new V ();
	}

	public static void Main ()
	{
		Test t = new Test ();
		t.v() [0] = 9;
	}
}
