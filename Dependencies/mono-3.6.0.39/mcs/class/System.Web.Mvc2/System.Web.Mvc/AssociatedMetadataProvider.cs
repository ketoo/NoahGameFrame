﻿/* ****************************************************************************
 *
 * Copyright (c) Microsoft Corporation. All rights reserved.
 *
 * This software is subject to the Microsoft Public License (Ms-PL). 
 * A copy of the license can be found in the license.htm file included 
 * in this distribution.
 *
 * You must not remove this notice, or any other, from this software.
 *
 * ***************************************************************************/

namespace System.Web.Mvc {
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.ComponentModel.DataAnnotations;
    using System.Globalization;
    using System.Linq;
    using System.Web.Mvc.Resources;

    // This class provides a good implementation of ModelMetadataProvider for people who will be
    // using traditional classes with properties. It uses the buddy class support from
    // DataAnnotations, and consolidates the three operations down to a single override
    // for reading the attribute values and creating the metadata class.
    public abstract class AssociatedMetadataProvider : ModelMetadataProvider {
        protected abstract ModelMetadata CreateMetadata(IEnumerable<Attribute> attributes, Type containerType, Func<object> modelAccessor, Type modelType, string propertyName);

        protected virtual IEnumerable<Attribute> FilterAttributes(Type containerType, PropertyDescriptor propertyDescriptor, IEnumerable<Attribute> attributes) {
            if (typeof(ViewPage).IsAssignableFrom(containerType) || typeof(ViewUserControl).IsAssignableFrom(containerType)) {
                return attributes.Where(a => !(a is ReadOnlyAttribute));
            }

            return attributes;
        }

        public override IEnumerable<ModelMetadata> GetMetadataForProperties(object container, Type containerType) {
            if (containerType == null) {
                throw new ArgumentNullException("containerType");
            }

            return GetMetadataForPropertiesImpl(container, containerType);
        }

        private IEnumerable<ModelMetadata> GetMetadataForPropertiesImpl(object container, Type containerType) {
            foreach (PropertyDescriptor property in GetTypeDescriptor(containerType).GetProperties()) {
                Func<object> modelAccessor = container == null ? null : GetPropertyValueAccessor(container, property);
                yield return GetMetadataForProperty(modelAccessor, containerType, property);
            }
        }

        public override ModelMetadata GetMetadataForProperty(Func<object> modelAccessor, Type containerType, string propertyName) {
            if (containerType == null) {
                throw new ArgumentNullException("containerType");
            }
            if (String.IsNullOrEmpty(propertyName)) {
                throw new ArgumentException(MvcResources.Common_NullOrEmpty, "propertyName");
            }

            ICustomTypeDescriptor typeDescriptor = GetTypeDescriptor(containerType);
            PropertyDescriptor property = typeDescriptor.GetProperties().Find(propertyName, true);
            if (property == null) {
                throw new ArgumentException(
                    String.Format(
                        CultureInfo.CurrentCulture,
                        MvcResources.Common_PropertyNotFound,
                        containerType.FullName, propertyName));
            }

            return GetMetadataForProperty(modelAccessor, containerType, property);
        }

        protected virtual ModelMetadata GetMetadataForProperty(Func<object> modelAccessor, Type containerType, PropertyDescriptor propertyDescriptor) {
            IEnumerable<Attribute> attributes = FilterAttributes(containerType, propertyDescriptor, propertyDescriptor.Attributes.Cast<Attribute>());
            return CreateMetadata(attributes, containerType, modelAccessor, propertyDescriptor.PropertyType, propertyDescriptor.Name);
        }

        public override ModelMetadata GetMetadataForType(Func<object> modelAccessor, Type modelType) {
            if (modelType == null) {
                throw new ArgumentNullException("modelType");
            }

            IEnumerable<Attribute> attributes = GetTypeDescriptor(modelType).GetAttributes().Cast<Attribute>();
            return CreateMetadata(attributes, null /* containerType */, modelAccessor, modelType, null /* propertyName */);
        }

        private static Func<object> GetPropertyValueAccessor(object container, PropertyDescriptor property) {
            return () => property.GetValue(container);
        }

        protected virtual ICustomTypeDescriptor GetTypeDescriptor(Type type) {
            return TypeDescriptorHelper.Get(type);
        }
    }
}
