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
using System.Collections;

namespace RabbitMQ.Client.Impl
{
    public abstract class BasicProperties: ContentHeaderBase, IBasicProperties
    {
        public abstract string ContentType { get; set; }
        public abstract string ContentEncoding { get; set; }
        public abstract IDictionary Headers { get; set; }
        public abstract byte DeliveryMode { get; set; }
        public abstract byte Priority { get; set; }
        public abstract string CorrelationId { get; set; }
        public abstract string ReplyTo { get; set; }
        public abstract string Expiration { get; set; }
        public abstract string MessageId { get; set; }
        public abstract AmqpTimestamp Timestamp { get; set; }
        public abstract string Type { get; set; }
        public abstract string UserId { get; set; }
        public abstract string AppId { get; set; }
        public abstract string ClusterId { get; set; }

        public abstract void ClearContentType();
        public abstract void ClearContentEncoding();
        public abstract void ClearHeaders();
        public abstract void ClearDeliveryMode();
        public abstract void ClearPriority();
        public abstract void ClearCorrelationId();
        public abstract void ClearReplyTo();
        public abstract void ClearExpiration();
        public abstract void ClearMessageId();
        public abstract void ClearTimestamp();
        public abstract void ClearType();
        public abstract void ClearUserId();
        public abstract void ClearAppId();
        public abstract void ClearClusterId();

        public abstract bool IsContentTypePresent();
        public abstract bool IsContentEncodingPresent();
        public abstract bool IsHeadersPresent();
        public abstract bool IsDeliveryModePresent();
        public abstract bool IsPriorityPresent();
        public abstract bool IsCorrelationIdPresent();
        public abstract bool IsReplyToPresent();
        public abstract bool IsExpirationPresent();
        public abstract bool IsMessageIdPresent();
        public abstract bool IsTimestampPresent();
        public abstract bool IsTypePresent();
        public abstract bool IsUserIdPresent();
        public abstract bool IsAppIdPresent();
        public abstract bool IsClusterIdPresent();

        public PublicationAddress ReplyToAddress
        {
            get
            {
                return PublicationAddress.Parse(ReplyTo);
            }
            set
            {
                ReplyTo = value.ToString();
            }
        }

        public void SetPersistent(bool persistent)
        {
            if (persistent)
                DeliveryMode = 2;
            else
                DeliveryMode = 1;
        }

        public override object Clone()
        {
            BasicProperties clone = MemberwiseClone() as BasicProperties;
            if (IsHeadersPresent())
            {
                clone.Headers = new Hashtable();
                foreach (DictionaryEntry entry in Headers)
                    clone.Headers[entry.Key] = entry.Value;
            }

            return clone;
        }
    }
}
