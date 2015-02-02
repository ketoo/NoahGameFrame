// This source code is dual-licensed under the Apache License, version
// 2.0, and the Mozilla Public License, version 1.1.
//
// The APL v2.0:
//
//---------------------------------------------------------------------------
//   Copyright (C) 2007-2010 LShift Ltd., Cohesive Financial
//   Technologies LLC., and Rabbit Technologies Ltd.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//---------------------------------------------------------------------------
//
// The MPL v1.1:
//
//---------------------------------------------------------------------------
//   The contents of this file are subject to the Mozilla Public License
//   Version 1.1 (the "License"); you may not use this file except in
//   compliance with the License. You may obtain a copy of the License at
//   http://www.rabbitmq.com/mpl.html
//
//   Software distributed under the License is distributed on an "AS IS"
//   basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
//   License for the specific language governing rights and limitations
//   under the License.
//
//   The Original Code is The RabbitMQ .NET Client.
//
//   The Initial Developers of the Original Code are LShift Ltd,
//   Cohesive Financial Technologies LLC, and Rabbit Technologies Ltd.
//
//   Portions created before 22-Nov-2008 00:00:00 GMT by LShift Ltd,
//   Cohesive Financial Technologies LLC, or Rabbit Technologies Ltd
//   are Copyright (C) 2007-2008 LShift Ltd, Cohesive Financial
//   Technologies LLC, and Rabbit Technologies Ltd.
//
//   Portions created by LShift Ltd are Copyright (C) 2007-2010 LShift
//   Ltd. Portions created by Cohesive Financial Technologies LLC are
//   Copyright (C) 2007-2010 Cohesive Financial Technologies
//   LLC. Portions created by Rabbit Technologies Ltd are Copyright
//   (C) 2007-2010 Rabbit Technologies Ltd.
//
//   All Rights Reserved.
//
//   Contributor(s): ______________________________________.
//
//---------------------------------------------------------------------------
using System;
using System.Runtime.Serialization;

namespace RabbitMQ.Client.Exceptions
{
    /// <summary>
    /// Thrown when a session is destroyed during an RPC call to a
    /// broker. For example, if a TCP connection dropping causes the
    /// destruction of a session in the middle of a QueueDeclare
    /// operation, an OperationInterruptedException will be thrown to
    /// the caller of IModel.QueueDeclare.
    /// </summary>
    [Serializable]
    public class OperationInterruptedException
        // Eventually, as part of .NET 3.0, we get to inherit from ...
        /* : OperationCanceledException */
        // Until then, there's sadly nothing better to inherit from than ...
        : Exception
    {
        ///<summary>Possible explanation for the interruption. May be null.</summary>
        protected ShutdownEventArgs m_shutdownReason;

        protected OperationInterruptedException() { }

        protected OperationInterruptedException(string message) : base(message) { }

        protected OperationInterruptedException(string message, System.Exception inner)
            : base(message, inner) { }

        protected OperationInterruptedException(SerializationInfo info,
                                                StreamingContext context)
            : base(info, context) { }

        ///<summary>Construct an OperationInterruptedException with
        ///the passed-in explanation, if any.</summary>
        public OperationInterruptedException(ShutdownEventArgs reason)
            : base(reason == null ? "The AMQP operation was interrupted" :
                   string.Format("The AMQP operation was interrupted: {0}",
                                 reason))
        {
            m_shutdownReason = reason;
        }

        ///<summary>Retrieves the explanation for the shutdown. May
        ///return null if no explanation is available.</summary>
        public ShutdownEventArgs ShutdownReason { get { return m_shutdownReason; } }
    }
}
