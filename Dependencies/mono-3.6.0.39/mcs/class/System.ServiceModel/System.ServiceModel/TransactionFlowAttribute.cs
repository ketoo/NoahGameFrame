//
// TransactionFlowAttribute.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2009 Novell, Inc.  http://www.novell.com
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
using System.ServiceModel.Channels;
using System.ServiceModel.Description;
using System.ServiceModel.Dispatcher;
using System.Transactions;

namespace System.ServiceModel
{
	[AttributeUsage (AttributeTargets.Method)]
	public sealed class TransactionFlowAttribute : Attribute, IOperationBehavior
	{
		public TransactionFlowAttribute (TransactionFlowOption transactions)
		{
			Transactions = transactions;
		}

		public TransactionFlowOption Transactions { get; private set; }

		void IOperationBehavior.AddBindingParameters (OperationDescription description, BindingParameterCollection parameters)
		{
		}

		[MonoTODO]
		void IOperationBehavior.ApplyClientBehavior (OperationDescription description, ClientOperation proxy)
		{
		}

		[MonoTODO]
		void IOperationBehavior.ApplyDispatchBehavior (OperationDescription description, DispatchOperation dispatch)
		{
		}

		[MonoTODO]
		void IOperationBehavior.Validate (OperationDescription description)
		{
		}
	}
}
