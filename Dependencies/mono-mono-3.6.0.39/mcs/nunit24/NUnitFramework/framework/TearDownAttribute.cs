// ****************************************************************
// This is free software licensed under the NUnit license. You
// may obtain a copy of the license as well as information regarding
// copyright ownership at http://nunit.org/?p=license&r=2.4.
// ****************************************************************

namespace NUnit.Framework
{
	using System;

	/// <summary>
	/// Attribute used to identify a method that is called 
	/// immediately after each test is run. The method is 
	/// guaranteed to be called, even if an exception is thrown.
	/// </summary>
	[AttributeUsage(AttributeTargets.Method, AllowMultiple=false)]
	public class TearDownAttribute : Attribute
	{}
}
