/*
  * Copyright (c) 2002-2004 Mainsoft Corporation.
  *
  * Permission is hereby granted, free of charge, to any person obtaining a
  * copy of this software and associated documentation files (the "Software"),
  * to deal in the Software without restriction, including without limitation
  * the rights to use, copy, modify, merge, publish, distribute, sublicense,
  * and/or sell copies of the Software, and to permit persons to whom the
  * Software is furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in
  * all copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  * DEALINGS IN THE SOFTWARE.
  */

using System;
using System.Collections;

using java.sql;

namespace System.Data.Common
{
	#region AbstractDbMetaDataCache

	internal abstract class AbstractDbMetaDataCache
	{
		Hashtable _cache;
		const int MINUTES_TIMEOUT = 10;
		private long _timestamp;

		protected AbstractDbMetaDataCache()
		{
			_cache = Hashtable.Synchronized(new Hashtable());
		}

		protected Hashtable Cache 
		{
			get
			{
				long now = DateTime.Now.Ticks;
				if (now - _timestamp > MINUTES_TIMEOUT * TimeSpan.TicksPerMinute)
				{
					_timestamp = now;
					_cache.Clear();
				}

				return _cache;
			}
		}
	}

	#endregion
}
