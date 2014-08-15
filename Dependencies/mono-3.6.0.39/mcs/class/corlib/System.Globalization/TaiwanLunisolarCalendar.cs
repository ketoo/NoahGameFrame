//
// TaiwanLunisolarCalendar.cs
//
// Author:
//	Atsushi Enomoto  <atsushi@ximian.com>
//
// Copyright (C) 2007 Novell, Inc.  http://www.novell.com
//

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

namespace System.Globalization {

using System;
using System.Runtime.InteropServices;

[Serializable]
public class TaiwanLunisolarCalendar : EastAsianLunisolarCalendar {
	internal static readonly CCEastAsianLunisolarEraHandler era_handler;

	static TaiwanLunisolarCalendar ()
	{
		era_handler = new CCEastAsianLunisolarEraHandler ();
		era_handler.appendEra(TaiwanEra, CCFixed.FromDateTime (TaiwanMin), CCFixed.FromDateTime (TaiwanMax));
	}

	const int TaiwanEra = 1;

	[MonoTODO]
	public TaiwanLunisolarCalendar ()
		: base (era_handler)
	{
	}

	public override int [] Eras {
		get {
			return (int []) era_handler.Eras.Clone();
		}
	}

	public override int GetEra (DateTime time) {
		// M_CheckDateTime not needed, because EraYear does the
		// right thing.
		int rd = CCFixed.FromDateTime(time);
		int era;
		era_handler.EraYear(out era, rd);
		return era;
	}

	static DateTime TaiwanMin = new DateTime (1912, 2, 18);
	static DateTime TaiwanMax = new DateTime (2051, 2, 10, 23, 59, 59, 999);
		
	public override DateTime MinSupportedDateTime {
		get {
			return TaiwanMin;
		}
	}

	public override DateTime MaxSupportedDateTime {
		get {
			return TaiwanMax;
		}
	}

#if NET_4_5
	protected override int DaysInYearBeforeMinSupportedYear {
		get{
			return 384;
		}
	}
#endif
}

}
