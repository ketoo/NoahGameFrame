﻿// -----------------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// -----------------------------------------------------------------------
using System;
using System.ComponentModel.Composition;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.UnitTesting;

namespace System.ComponentModel.Composition
{
    [TestClass]
    public class PartMetadataAttributeTests
    {
        [TestMethod]
        public void Constructor_NullAsNameArgument_ShouldSetNamePropertyToEmptyString()
        {
            var attribute = new PartMetadataAttribute((string)null, "Value");

            Assert.AreEqual(string.Empty, attribute.Name);
        }

        [TestMethod]
        public void Constructor_ValueAsNameArgument_ShouldSetNameProperty()
        {
            var expectations = Expectations.GetMetadataNames();

            foreach (var e in expectations)
            {
                var attribute = new PartMetadataAttribute(e, "Value");

                Assert.AreEqual(e, attribute.Name);
            }
        }

        [TestMethod]
        public void Constructor_ValueAsValueArgument_ShouldSetValueProperty()
        {
            var expectations = Expectations.GetMetadataValues();

            foreach (var e in expectations)
            {
                var attribute = new PartMetadataAttribute("Name", e);

                Assert.AreEqual(e, attribute.Value);
            }
        }
    }
}
