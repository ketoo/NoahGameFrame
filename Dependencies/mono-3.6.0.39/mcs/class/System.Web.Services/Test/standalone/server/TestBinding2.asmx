<%@ WebService Language="c#" Codebehind="GetData.wsdl.cs" Class="GetData.wsdl.GetData" %>

using System.Xml.Serialization;
using System;
using System.Web.Services.Protocols;
using System.ComponentModel;
using System.Web.Services;

namespace GetData.wsdl{

	[System.Web.Services.WebServiceBindingAttribute(Name="GetDataSoap2", Namespace="http://anotheruri/")]
	public class GetData : System.Web.Services.WebService {

  		
		[System.Web.Services.WebMethodAttribute()]
    		public bool RebuildTabList(bool X_WS_ReturnValue_X)
		{
			return false;
		}

		[System.Web.Services.WebMethodAttribute()]
		[SoapDocumentMethod (Binding="GetDataSoap2")]
    		public bool OtherMethod(bool X_WS_ReturnValue_X)
		{
			return false;
		}

	}
}
