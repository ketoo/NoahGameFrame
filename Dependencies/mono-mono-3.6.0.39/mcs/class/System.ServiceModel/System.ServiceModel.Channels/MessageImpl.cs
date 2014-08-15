//
// MessageImpl.cs
//
// Author:
//	Atsushi Enomoto <atsushi@ximian.com>
//
// Copyright (C) 2006 Novell, Inc.  http://www.novell.com
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
using System.Linq;
using System.Runtime.Serialization;
using System.Xml;
using System.IO;

namespace System.ServiceModel.Channels
{
	internal class AttributeInfo
	{
		public string Prefix, Name, Namespace, Value;
	}
	
	internal class AttributeCollection : List<AttributeInfo>
	{
		public AttributeCollection ()
		{
		}

		public AttributeCollection (AttributeCollection copy)
			: base (copy)
		{
		}
	}

	internal class XmlReaderMessage : Message
	{
		MessageVersion version;
		XmlDictionaryReader reader;
		MessageHeaders headers;
		MessageProperties properties = new MessageProperties ();
		bool is_empty, is_fault, body_started, body_consumed;
		int max_headers;
		AttributeCollection attributes;

		public XmlReaderMessage (MessageVersion version, XmlDictionaryReader reader, int maxSizeOfHeaders)
		{
			this.version = version;
			this.reader = reader;
			this.max_headers = maxSizeOfHeaders;

			ReadEnvelopeStart ();
		}

		public override MessageHeaders Headers {
			get {
				if (headers == null)
					ReadHeaders ();
				return headers;
			}
		}

		public override bool IsEmpty {
			get {
				ReadBodyStart ();
				return is_empty;
			}
		}

		public override bool IsFault {
			get {
				ReadBodyStart ();
				return is_fault;
			}
		}

		public override MessageProperties Properties {
			get { return properties; }
		}

		public override MessageVersion Version {
			get { return version; }
		}

		protected override MessageBuffer OnCreateBufferedCopy (
			int maxBufferSize)
		{
			ReadBodyStart ();
			var headers = new MessageHeaders (Headers);
			var props = new MessageProperties (Properties);
			if (IsEmpty)
				return new DefaultMessageBuffer (headers, props, attributes);
			else
				return new DefaultMessageBuffer (maxBufferSize, headers, props, new XmlReaderBodyWriter (reader), IsFault, attributes);
		}

		protected override string OnGetBodyAttribute (
			string localName, string ns)
		{
			ReadBodyStart ();
			var att = attributes.FirstOrDefault (a => a.Name == localName && a.Namespace == ns);
			return att != null ? att.Value : null;
		}

		protected override XmlDictionaryReader OnGetReaderAtBodyContents ()
		{
			if (reader.ReadState == ReadState.Closed)
				return reader; // silly, but that's what our test expects.
			ReadBodyStart ();
			if (is_empty)
				throw new InvalidOperationException ("The message body is empty.");
			body_consumed = true;
			return reader;
		}

		protected override void OnWriteBodyContents (
			XmlDictionaryWriter writer)
		{
			XmlDictionaryReader reader = GetReaderAtBodyContents ();
			while (!reader.EOF && reader.NodeType != XmlNodeType.EndElement)
				writer.WriteNode (reader, false);
		}

		static readonly char [] whitespaceChars = new char [] {' ', '\t', '\r', '\n'};

		void ReadEnvelopeStart ()
		{
			reader.MoveToContent ();
			if (reader.IsEmptyElement)
				throw new ArgumentException ("Missing message content XML.");
			reader.ReadStartElement ("Envelope", Version.Envelope.Namespace);

			// SOAP Header
			reader.MoveToContent ();
		}

		void ReadHeaders ()
		{
			if (headers != null)
				throw new InvalidOperationException ("XmlReader at headers is already consumed.");

			string envNS = Version.Envelope.Namespace;

			headers = new MessageHeaders (version);
			if (reader.LocalName != "Header" || reader.NamespaceURI != envNS)
				return;

			bool isEmptyHeader = reader.IsEmptyElement;
			reader.ReadStartElement ("Header", envNS);
			reader.MoveToContent ();
			if (isEmptyHeader)
				return;

			int nHeaders = 0;
			while (!reader.EOF && reader.NodeType != XmlNodeType.EndElement) {
				if (reader.NodeType == XmlNodeType.Element) {
					if (nHeaders++ == max_headers)
						throw new InvalidOperationException (String.Format ("Message header size has exceeded the maximum header size {0}", max_headers));
					headers.Add (new MessageHeader.XmlMessageHeader (reader, Version));
				}
				else
					reader.Skip ();
				// FIXME: handle UnderstoodHeaders as well.
				reader.MoveToContent ();
			}
			reader.ReadEndElement ();
			reader.MoveToContent ();
		}

		void ReadBodyStart ()
		{
			if (body_consumed)
				throw new InvalidOperationException ("The message body XmlReader is already consumed.");

			if (body_started)
				return;

			// read headers in advance.
			if (headers == null)
				ReadHeaders ();

			// SOAP Body
			body_started = true;
			is_empty = reader.IsEmptyElement;
			attributes = new AttributeCollection ();
			reader.MoveToContent ();
			if (reader.MoveToFirstAttribute ()) {
				do {
					attributes.Add (new AttributeInfo () { Prefix = reader.Prefix, Name = reader.LocalName, Namespace = reader.NamespaceURI, Value = reader.Value});
				} while (reader.MoveToNextAttribute ());
				reader.MoveToElement ();
			}
			reader.ReadStartElement ("Body", Version.Envelope.Namespace);
			if (reader.NodeType == XmlNodeType.EndElement) {
				is_empty = true;
				reader.Read ();
			} else {
				reader.MoveToContent ();
				if (reader.NodeType == XmlNodeType.Element &&
				    reader.LocalName == "Fault" &&
				    reader.NamespaceURI == Version.Envelope.Namespace)
					is_fault = true;
			}
		}

		protected override void OnWriteStartBody (
			XmlDictionaryWriter writer)
		{
			ReadBodyStart (); // consume up to attributes

			base.OnWriteStartBody (writer);
			foreach (var p in attributes)
				writer.WriteAttributeString (p.Prefix, p.Name, p.Namespace, p.Value);
		}
	}

	internal abstract class MessageImplBase : Message
	{
		MessageHeaders headers;
		MessageProperties properties = new MessageProperties ();
		AttributeCollection attributes;

		public MessageImplBase (MessageVersion version, string action, AttributeCollection attributes)
		{
			headers = new MessageHeaders (version);
			if (action != null)
				headers.Action = action;
			this.attributes = attributes;
		}

		internal AttributeCollection Attributes {
			get { return attributes; }
		}

		public override MessageHeaders Headers {
			get { return headers; }
		}

		public override MessageProperties Properties {
			get { return properties; }
		}

		public override MessageVersion Version {
			get { return Headers.MessageVersion; }
		}

		protected override string OnGetBodyAttribute (
			string localName, string ns)
		{
			var att = attributes.FirstOrDefault (a => a.Name == localName && a.Namespace == ns);
			return att != null ? att.Value : null;
		}

		protected override void OnWriteStartBody (
			XmlDictionaryWriter writer)
		{
			var dic = Constants.SoapDictionary;
			writer.WriteStartElement ("s", dic.Add ("Body"), dic.Add (Version.Envelope.Namespace));
			foreach (var p in Attributes)
				writer.WriteAttributeString (p.Prefix, p.Name, p.Namespace, p.Value);
		}
	}

	internal class EmptyMessage : MessageImplBase
	{
		static readonly AttributeCollection empty_attributes = new AttributeCollection ();

		public EmptyMessage (MessageVersion version, string action)
			: base (version, action, empty_attributes)
		{
		}

		public override bool IsEmpty {
			get { return true; }
		}

		protected override void OnWriteBodyContents (
			XmlDictionaryWriter writer)
		{
		}

		protected override MessageBuffer OnCreateBufferedCopy (
			int maxBufferSize)
		{
			return new DefaultMessageBuffer (Headers, Properties, Attributes);
		}
	}

	internal class SimpleMessage : MessageImplBase
	{
		BodyWriter body;
		bool is_fault;

		public SimpleMessage (MessageVersion version,
			string action, BodyWriter body, bool isFault, AttributeCollection attributes)
			: base (version, action, attributes)
		{
			this.body = body;
			this.is_fault = isFault;
		}

		public override bool IsEmpty {
			get { return false; }
		}

		public override bool IsFault {
			get { return is_fault; }
		}

		protected override void OnWriteBodyContents (
			XmlDictionaryWriter writer)
		{
			body.WriteBodyContents (writer);
		}

		protected override MessageBuffer OnCreateBufferedCopy (
			int maxBufferSize)
		{
			var headers = new MessageHeaders (Headers);
			var props = new MessageProperties (Properties);
			var atts = new AttributeCollection (Attributes);
			return new DefaultMessageBuffer (maxBufferSize, headers, props, body.CreateBufferedCopy (maxBufferSize), IsFault, atts);
		}
	}
}

