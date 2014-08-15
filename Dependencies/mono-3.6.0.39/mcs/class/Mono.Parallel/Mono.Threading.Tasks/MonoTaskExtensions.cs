// 
// MonoTaskExtensions.cs
//  
// Author:
//       Jérémie "Garuma" Laval <jeremie.laval@gmail.com>
// 
// Copyright (c) 2011 Jérémie "Garuma" Laval
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#if NET_4_0

using System;
using System.Threading.Tasks;
using System.Reflection;

namespace Mono.Threading.Tasks
{
	public static class MonoTaskExtensions
	{
		readonly static Action<Task, Action<Task>> internalExecute = null;

		static MonoTaskExtensions ()
		{
			// Initialize internal execute
			var method = typeof(Task).GetMethod ("Execute", BindingFlags.Instance | BindingFlags.NonPublic);
			internalExecute = (Action<Task, Action<Task>>)Delegate.CreateDelegate (typeof(Action<Task, Action<Task>>), method);
		}

		// Allow external worker to call into the otherwise internal corresponding method of Task
		public static void Execute (this Task task, Action<Task> childWorkAdder)
		{
			internalExecute (task, childWorkAdder);
		}
	}
}

#endif
