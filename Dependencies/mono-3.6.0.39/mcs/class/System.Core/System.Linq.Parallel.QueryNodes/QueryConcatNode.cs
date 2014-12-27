//
// QueryConcatNode.cs
//
// Author:
//       Jérémie "Garuma" Laval <jeremie.laval@gmail.com>
//
// Copyright (c) 2010 Jérémie "Garuma" Laval
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
using System.Threading;
using System.Collections;
using System.Collections.Generic;

namespace System.Linq.Parallel.QueryNodes
{
	internal class QueryConcatNode<TSource> : QueryMuxNode<TSource, TSource, TSource>
	{
		public QueryConcatNode (QueryBaseNode<TSource> first, QueryBaseNode<TSource> second)
			: base (first, second)
		{
		}

		internal override IList<IEnumerable<TSource>> GetEnumerables (QueryOptions options)
		{
			var second = Second.GetEnumerables (options);

			return Parent.GetEnumerables (options)
				.Select ((f, i) => CombineEnumerables (f, second[i]))
				.ToList ();
		}

		internal override IList<IEnumerable<KeyValuePair<long, TSource>>> GetOrderedEnumerables (QueryOptions options)
		{
			var second = Second.GetOrderedEnumerables (options);

			return Parent.GetOrderedEnumerables (options)
				.Select ((f, i) => CombineEnumerables (f, second[i]))
				.ToList ();
		}

		internal override IEnumerable<TSource> GetSequential ()
		{
			IEnumerable<TSource> first = Parent.GetSequential ();
			IEnumerable<TSource> second = Second.GetSequential ();

			return first.Concat (second);
		}

		IEnumerable<TResult> CombineEnumerables<TResult> (IEnumerable<TResult> f, IEnumerable<TResult> s)
		{
			foreach (var e in f)
				yield return e;
			foreach (var e in s)
				yield return e;
		}
	}
}

#endif
