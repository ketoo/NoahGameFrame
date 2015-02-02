// HtmlAgilityPack V1.0 - Simon Mourier <simonm@microsoft.com>
using System.Reflection;
using System.Runtime.CompilerServices;

//
// General Information about an assembly is controlled through the following 
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
//
#if DEBUG
[assembly: AssemblyTitle("Html Agility Pack - Debug")] //Description
#else // release
#if TRACE
[assembly: AssemblyTitle("Html Agility Pack - ReleaseTrace")] //Description
#else
[assembly: AssemblyTitle("Html Agility Pack - Release")] //Description
#endif
#endif
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("Simon Mourier")]
[assembly: AssemblyProduct("Html Agility Pack")]
[assembly: AssemblyCopyright("Simon Mourier")] 
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]		

//
// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version 
//      Build Number
//      Revision
//
// You can specify all the values or you can default the Revision and Build Numbers 
// by using the '*' as shown below:

[assembly: AssemblyVersion("1.3.0.0")]
[assembly: AssemblyInformationalVersion("1.3.1.1")]

//
// In order to sign your assembly you must specify a key to use. Refer to the 
// Microsoft .NET Framework documentation for more information on assembly signing.
//
// Use the attributes below to control which key is used for signing. 
//
// Notes: 
//   (*) If no key is specified, the assembly is not signed.
//   (*) KeyName refers to a key that has been installed in the Crypto Service
//       Provider (CSP) on your machine. KeyFile refers to a file which contains
//       a key.
//   (*) If the KeyFile and the KeyName values are both specified, the 
//       following processing occurs:
//       (1) If the KeyName can be found in the CSP, that key is used.
//       (2) If the KeyName does not exist and the KeyFile does exist, the key 
//           in the KeyFile is installed into the CSP and used.
//   (*) In order to create a KeyFile, you can use the sn.exe (Strong Name) utility.
//       When specifying the KeyFile, the location of the KeyFile should be
//       relative to the project output directory which is
//       %Project Directory%\obj\<configuration>. For example, if your KeyFile is
//       located in the project directory, you would specify the AssemblyKeyFile 
//       attribute as [assembly: AssemblyKeyFile("..\\..\\mykey.snk")]
//   (*) Delay Signing is an advanced option - see the Microsoft .NET Framework
//       documentation for more information on this.
//
#if !(TARGET_JVM || TARGET_DOTNET)
[assembly: AssemblyDelaySign (true)]
[assembly: AssemblyKeyFile ("../winfx.pub")]
#endif
