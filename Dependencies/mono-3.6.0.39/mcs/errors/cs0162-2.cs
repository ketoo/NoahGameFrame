// CS0162: Unreachable code detected
// Line: 18
// Compiler options: -warnaserror -warn:2

using System;

class C {
	public enum Flags {
		Removed	= 0,
		Public	= 1
	}

	public Flags	_enumFlags;
		
	internal void Close()
	{	
		if ((_enumFlags & Flags.Removed) != 0)
			Console.WriteLine ("error");
	}
}
