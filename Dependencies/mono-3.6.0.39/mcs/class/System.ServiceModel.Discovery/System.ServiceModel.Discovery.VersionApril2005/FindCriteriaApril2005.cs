//
// Author: Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2009,2010 Novell, Inc (http://www.novell.com)
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
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.ServiceModel.Description;
using System.ServiceModel.Dispatcher;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;

namespace System.ServiceModel.Discovery.VersionApril2005
{
	[XmlSchemaProvider ("GetSchema")]
	public class FindCriteriaApril2005 : IXmlSerializable
	{
		public static FindCriteriaApril2005 FromFindCriteria (FindCriteria findCriteria)
		{
			return new FindCriteriaApril2005 (findCriteria);
		}

		static readonly DiscoveryVersion version = DiscoveryVersion.WSDiscoveryApril2005;
		static XmlSchema schema = FindCriteria.BuildSchema (version);

		public static XmlQualifiedName GetSchema (XmlSchemaSet schemaSet)
		{
			schemaSet.Add (schema);
			return new XmlQualifiedName ("ProbeType", version.Namespace);
		}
		
		// for deserialization
		FindCriteriaApril2005 ()
		{
		}

		internal FindCriteriaApril2005 (FindCriteria source)
		{
			this.source = source;
		}

		FindCriteria source;

		public XmlSchema GetSchema ()
		{
			return null;
		}

		public void ReadXml (XmlReader reader)
		{
			source = FindCriteria.ReadXml (reader, version);
		}

		public FindCriteria ToFindCriteria ()
		{
			if (source == null)
				throw new InvalidOperationException ("Call ReadXml method before calling this method.");
			return source;
		}

		public void WriteXml (XmlWriter writer)
		{
			source.WriteXml (writer, version);
		}
	}
}
