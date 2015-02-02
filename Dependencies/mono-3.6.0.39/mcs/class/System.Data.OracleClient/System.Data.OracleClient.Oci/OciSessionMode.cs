// 
// OciSessionMode.cs 
//  
// Part of managed C#/.NET library System.Data.OracleClient.dll
//
// Part of the Mono class libraries at
// mcs/class/System.Data.OracleClient/System.Data.OracleClient.Oci
//
// Assembly: System.Data.OracleClient.dll
// Namespace: System.Data.OracleClient.Oci
// 
// Author: 
//     Tim Coleman <tim@timcoleman.com>
//         
// Copyright (C) Tim Coleman, 2003
// 

using System;

namespace System.Data.OracleClient.Oci {
	[Flags]
	internal enum OciSessionMode {
		Default = 0x00,
		Migrate = 0x01,
		SysDBA = 0x02,
		SysOper = 0x04,
		PrelimAuth = 0x08
	}
}
