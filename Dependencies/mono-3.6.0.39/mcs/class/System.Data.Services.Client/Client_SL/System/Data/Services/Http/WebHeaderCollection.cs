//Copyright 2010 Microsoft Corporation
//
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
//You may obtain a copy of the License at 
//
//http://www.apache.org/licenses/LICENSE-2.0 
//
//Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an 
//"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
//See the License for the specific language governing permissions and limitations under the License.


namespace System.Data.Services.Http
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;

    internal abstract class WebHeaderCollection
    {
        #region Properties.

        public abstract int Count
        {
            get;
        }

        public abstract ICollection<string> AllKeys
        {
            get;
        }

        public abstract string this[string name]
        {
            get;
            set;
        }

        public abstract string this[System.Data.Services.Http.HttpRequestHeader header]
        {
            get;
            set;
        }
        #endregion Properties.

        public virtual void Set(HttpRequestHeader header, string value)
        {
            this[header] = value;
        }
    }
}
