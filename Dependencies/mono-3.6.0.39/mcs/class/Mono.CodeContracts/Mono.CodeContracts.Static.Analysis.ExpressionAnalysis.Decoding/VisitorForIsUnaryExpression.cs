// 
// VisitorForIsUnaryExpression.cs
// 
// Authors:
// 	Alexander Chebaturkin (chebaturkin@gmail.com)
// 
// Copyright (C) 2011 Alexander Chebaturkin
// 
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//  
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 

using System;
using Mono.CodeContracts.Static.AST;
using Mono.CodeContracts.Static.DataStructures;

namespace Mono.CodeContracts.Static.Analysis.ExpressionAnalysis.Decoding {
	class VisitorForIsUnaryExpression<V, E> : QueryVisitor<V, E>
		where V : IEquatable<V>
		where E : IEquatable<E> {
		private E Argument;
		private UnaryOperator Operator;

		public static bool IsUnary (E expr, out UnaryOperator op, out E arg, FullExpressionDecoder<V, E> decoder)
		{
			VisitorForIsUnaryExpression<V, E> v = decoder.UnaryExpressionVisitor;
			bool res = Decode (expr, v, decoder);
			op = v.Operator;
			arg = v.Argument;
			return res;
		}

		public override bool Unary (E pc, UnaryOperator op, bool unsigned, V dest, E source, Dummy data)
		{
			this.Argument = source;
			this.Operator = op;
			return true;
		}
	}
}