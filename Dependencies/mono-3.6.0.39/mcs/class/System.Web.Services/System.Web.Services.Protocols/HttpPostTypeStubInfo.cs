//
// HttpPostTypeStubInfo.cs: Information about a method and its mapping to a SOAP web service.
//
// Author:
//   Lluis Sanchez Gual (lluis@ximian.com)
//
// (C) 2003 Ximian, Inc.
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

namespace System.Web.Services.Protocols 
{
	internal class HttpPostMethodStubInfo : HttpSimpleMethodStubInfo
	{
		public HttpPostMethodStubInfo (TypeStubInfo parent, LogicalMethodInfo source): base (parent, source)
		{
			ParameterReaderType = new MimeFormatterInfo (typeof(HtmlFormParameterReader));
			ReturnWriterType = new MimeFormatterInfo (typeof(XmlReturnWriter));

			if (ParameterWriterType == null) ParameterWriterType = new MimeFormatterInfo (typeof(HtmlFormParameterWriter));
		}
	}
	
	internal class HttpPostTypeStubInfo : HttpSimpleTypeStubInfo
	{
		public HttpPostTypeStubInfo (LogicalTypeInfo logicalTypeInfo): base (logicalTypeInfo)
		{
		}

		public override string ProtocolName
		{
			get { return "HttpPost"; }
		}
		
		protected override MethodStubInfo CreateMethodStubInfo (TypeStubInfo typeInfo, LogicalMethodInfo methodInfo, bool isClientProxy)
		{
			if (isClientProxy && methodInfo.ActualMethodInfo.GetCustomAttributes (typeof(HttpMethodAttribute),true).Length == 0) return null;
			if (!ValueCollectionParameterReader.IsSupported (methodInfo)) return null;
			return new HttpPostMethodStubInfo (typeInfo, methodInfo);
		}
	}
}
