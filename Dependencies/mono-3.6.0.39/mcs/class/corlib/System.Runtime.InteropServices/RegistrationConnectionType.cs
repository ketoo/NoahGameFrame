//
// System.Runtime.InteropServices.RegistrationConnectionType
//
// Author:
//   Kazuki Oikawa  (kazuki@panicode.com)
//

using System;

namespace System.Runtime.InteropServices
{
	[Flags]
	public enum RegistrationConnectionType
	{
		MultipleUse = 1,
		MultiSeparate = 2,
		SingleUse = 0,
		Suspended = 4,
		Surrogate = 8
	}
}
