//
// QueryCheckerVisitor.cs
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
using System.Linq.Parallel.QueryNodes;

namespace System.Linq.Parallel
{
	using OptionsList = Tuple<ParallelMergeOptions?, ParallelExecutionMode?, CancellationToken?, int, CancellationTokenSource>;

	internal class QueryCheckerVisitor : INodeVisitor
	{
		const int minSequentialThreshold = 20;

		// Information gathering
		ParallelMergeOptions? options = null;
		ParallelExecutionMode? mode = null;
		CancellationToken? token = null;
		int? degreeOfParallelism = null;
		CancellationToken implementerToken = CancellationToken.None;

		int partitionCount;
		bool? behindOrderGuard = null;

		internal QueryCheckerVisitor (int partitionCount)
		{
			this.partitionCount = partitionCount;
		}

		#region INodeVisitor implementation
		public void Visit (QueryBaseNode node)
		{
			// Nothing to do atm. Later we can check if the node is a
			// Take or a Skip and set accordingly UseStrip
		}

		public void Visit (QueryChildNode node)
		{
			node.Parent.Visit (this);
		}

		public void Visit (QueryOptionNode node)
		{
			MergeOptions (node.GetOptions ());

			Visit ((QueryChildNode)node);
		}

		public void Visit (QueryStartNode node)
		{
			if (behindOrderGuard == null)
				behindOrderGuard = false;
			if (degreeOfParallelism != null)
				partitionCount = degreeOfParallelism.Value;

			int count;
			if ((count = node.Count) != -1 && count < minSequentialThreshold)
				ShouldBeSequential = true;
		}

		public void Visit (QueryStreamNode node)
		{
			if (node.IsIndexed)
				UseStrip = true;

			Visit ((QueryChildNode)node);
		}

		public void Visit (QueryOrderGuardNode node)
		{
			if (behindOrderGuard == null) {
				if (node.EnsureOrder) {
					behindOrderGuard = true;
					//UseStrip = true;
				} else {
					behindOrderGuard = false;
				}
			}

			Visit ((QueryStreamNode)node);
		}

		public void Visit (QueryMuxNode node)
		{
			Visit ((QueryChildNode)node);
		}

		public void Visit (QueryHeadWorkerNode node)
		{
			// Wouldn't it be better with standard Linq?
			if (node.Count.HasValue && node.Count < partitionCount)
				ShouldBeSequential = true;

			Visit ((QueryStreamNode)node);
		}
		#endregion

		internal QueryOptions Options {
			get {
				return new QueryOptions (options, mode, token == null ? CancellationToken.None : token.Value,
				                         UseStrip, behindOrderGuard, partitionCount, implementerToken, ShouldBeSequential);
			}
		}

		internal bool UseStrip {
			get;
			private set;
		}

		internal bool BehindOrderGuard {
			get {
				return behindOrderGuard.Value;
			}
		}

		internal bool ShouldBeSequential {
			get;
			private set;
		}

		void MergeOptions (OptionsList list)
		{
			if (list.Item1 != null) {
				if (options == null)
					options = list.Item1;
				else
					Throw ("WithMergeOptions");
			}

			if (list.Item2 != null) {
				if (mode == null)
					mode = list.Item2;
				else
					Throw ("WithExecutionMode");
			}

			if (list.Item3 != null) {
				if (token == null)
					token = list.Item3;
				else
					Throw ("WithCancellationToken");
			}

			if (list.Item4 != -1) {
				if (degreeOfParallelism == null)
					degreeOfParallelism = list.Item4;
				else
					Throw ("WithDegreeOfParallelism");
			}

			// That one is treated specially
			if (list.Item5 != null) {
				implementerToken = implementerToken.Chain (list.Item5);
			}
		}

		void Throw (string methName)
		{
			throw new InvalidOperationException ("You can't have more than one " + methName + " node in a query");
		}
	}
}
#endif
