// CS0122: `Const.Version' is inaccessible due to its protection level
// Line: 6

using System.Reflection;

[assembly: AssemblyVersion(Const.Version)]

class Const
{
	const string Version = "0.1";
}
