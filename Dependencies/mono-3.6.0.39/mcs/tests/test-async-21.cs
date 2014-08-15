using System;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

class S
{
	public A GetAwaiter ()
	{
		return new A ();
	}
}

class A : INotifyCompletion
{
	bool IsCompleted {
		get {
			return false;
		}
	}
	
	void INotifyCompletion.OnCompleted (Action a)
	{
		a ();
	}
	
	int GetResult ()
	{
		return 3;
	}
	
	static async Task<int> Test1 ()
	{
		dynamic d = new S ();
		return await d;
	}

	public static int Main ()
	{
		var r = Test1 ();
		if (!Task.WaitAll (new [] { r }, 1000))
			return 1;
		
		Console.WriteLine ("ok");
		return 0;
	}
}