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
    using System.Collections;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Diagnostics.CodeAnalysis;
    using System.Globalization;
    using System.Reflection;
    using System.Web.Mvc.Resources;

    // TODO: Unit test ModelState interaction with VDD

    public class ViewDataDictionary : IDictionary<string, object> {

        private readonly Dictionary<string, object> _innerDictionary = new Dictionary<string, object>(StringComparer.OrdinalIgnoreCase);
        private object _model;
        private readonly ModelStateDictionary _modelState = new ModelStateDictionary();

        public ViewDataDictionary()
            : this((object)null) {
        }

        [SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors",
            Justification = "See note on SetModel() method.")]
        public ViewDataDictionary(object model) {
            Model = model;
        }

        [SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors",
            Justification = "See note on SetModel() method.")]
        public ViewDataDictionary(ViewDataDictionary dictionary) {
            if (dictionary == null) {
                throw new ArgumentNullException("dictionary");
            }

            foreach (var entry in dictionary) {
                _innerDictionary.Add(entry.Key, entry.Value);
            }
            foreach (var entry in dictionary.ModelState) {
                ModelState.Add(entry.Key, entry.Value);
            }
            Model = dictionary.Model;
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public int Count {
            get {
                return _innerDictionary.Count;
            }
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public bool IsReadOnly {
            get {
                return ((IDictionary<string, object>)_innerDictionary).IsReadOnly;
            }
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public ICollection<string> Keys {
            get {
                return _innerDictionary.Keys;
            }
        }

        public object Model {
            get {
                return _model;
            }
            set {
                SetModel(value);
            }
        }

        public ModelStateDictionary ModelState {
            get {
                return _modelState;
            }
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public object this[string key] {
            get {
                object value;
                _innerDictionary.TryGetValue(key, out value);
                return value;
            }
            set {
                _innerDictionary[key] = value;
            }
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public ICollection<object> Values {
            get {
                return _innerDictionary.Values;
            }
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public void Add(KeyValuePair<string, object> item) {
            ((IDictionary<string, object>)_innerDictionary).Add(item);
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public void Add(string key, object value) {
            _innerDictionary.Add(key, value);
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public void Clear() {
            _innerDictionary.Clear();
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public bool Contains(KeyValuePair<string, object> item) {
            return ((IDictionary<string, object>)_innerDictionary).Contains(item);
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public bool ContainsKey(string key) {
            return _innerDictionary.ContainsKey(key);
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public void CopyTo(KeyValuePair<string, object>[] array, int arrayIndex) {
            ((IDictionary<string, object>)_innerDictionary).CopyTo(array, arrayIndex);
        }

        [SuppressMessage("Microsoft.Naming", "CA1704:IdentifiersShouldBeSpelledCorrectly", MessageId = "Eval",
            Justification = "Commonly used shorthand for Evaluate.")]
        public object Eval(string expression) {
            if (String.IsNullOrEmpty(expression)) {
                throw new ArgumentException(MvcResources.Common_NullOrEmpty, "expression");
            }

            return ViewDataEvaluator.Eval(this, expression);
        }

        [SuppressMessage("Microsoft.Naming", "CA1704:IdentifiersShouldBeSpelledCorrectly", MessageId = "Eval",
            Justification = "Commonly used shorthand for Evaluate.")]
        public string Eval(string expression, string format) {
            object value = Eval(expression);

            if (value == null) {
                return String.Empty;
            }

            if (String.IsNullOrEmpty(format)) {
                return Convert.ToString(value, CultureInfo.CurrentCulture);
            }
            else {
                return String.Format(CultureInfo.CurrentCulture, format, value);
            }
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
            return _innerDictionary.GetEnumerator();
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public bool Remove(KeyValuePair<string, object> item) {
            return ((IDictionary<string, object>)_innerDictionary).Remove(item);
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public bool Remove(string key) {
            return _innerDictionary.Remove(key);
        }

        // This method will execute before the derived type's instance constructor executes. Derived types must
        // be aware of this and should plan accordingly. For example, the logic in SetModel() should be simple
        // enough so as not to depend on the "this" pointer referencing a fully constructed object.
        protected virtual void SetModel(object value) {
            _model = value;
        }

        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        public bool TryGetValue(string key, out object value) {
            return _innerDictionary.TryGetValue(key, out value);
        }

        internal static class ViewDataEvaluator {
            public static object Eval(ViewDataDictionary vdd, string expression) {
                //Given an expression "foo.bar.baz" we look up the following (pseudocode):
                //  this["foo.bar.baz.quux"]
                //  this["foo.bar.baz"]["quux"]
                //  this["foo.bar"]["baz.quux]
                //  this["foo.bar"]["baz"]["quux"]
                //  this["foo"]["bar.baz.quux"]
                //  this["foo"]["bar.baz"]["quux"]
                //  this["foo"]["bar"]["baz.quux"]
                //  this["foo"]["bar"]["baz"]["quux"]

                object evaluated = EvalComplexExpression(vdd, expression);
                return evaluated;
            }

            private static object EvalComplexExpression(object indexableObject, string expression) {
                foreach (ExpressionPair expressionPair in GetRightToLeftExpressions(expression)) {
                    string subExpression = expressionPair.Left;
                    string postExpression = expressionPair.Right;

                    object subtarget = GetPropertyValue(indexableObject, subExpression);
                    if (subtarget != null) {
                        if (String.IsNullOrEmpty(postExpression))
                            return subtarget;

                        object potential = EvalComplexExpression(subtarget, postExpression);
                        if (potential != null) {
                            return potential;
                        }
                    }
                }
                return null;
            }

            private static IEnumerable<ExpressionPair> GetRightToLeftExpressions(string expression) {
                // Produces an enumeration of all the combinations of complex property names
                // given a complex expression. See the list above for an example of the result
                // of the enumeration.

                yield return new ExpressionPair(expression, String.Empty);

                int lastDot = expression.LastIndexOf('.');

                string subExpression = expression;
                string postExpression = string.Empty;

                while (lastDot > -1) {
                    subExpression = expression.Substring(0, lastDot);
                    postExpression = expression.Substring(lastDot + 1);
                    yield return new ExpressionPair(subExpression, postExpression);

                    lastDot = subExpression.LastIndexOf('.');
                }
            }

            private static object GetIndexedPropertyValue(object indexableObject, string key) {
                Type indexableType = indexableObject.GetType();

                ViewDataDictionary vdd = indexableObject as ViewDataDictionary;
                if (vdd != null) {
                    return vdd[key];
                }

                MethodInfo containsKeyMethod = indexableType.GetMethod("ContainsKey", BindingFlags.Public | BindingFlags.Instance, null, new Type[] { typeof(string) }, null);
                if (containsKeyMethod != null) {
                    if (!(bool)containsKeyMethod.Invoke(indexableObject, new object[] { key })) {
                        return null;
                    }
                }

                PropertyInfo info = indexableType.GetProperty("Item", BindingFlags.Public | BindingFlags.Instance, null, null, new Type[] { typeof(string) }, null);
                if (info != null) {
                    return info.GetValue(indexableObject, new object[] { key });
                }

                PropertyInfo objectInfo = indexableType.GetProperty("Item", BindingFlags.Public | BindingFlags.Instance, null, null, new Type[] { typeof(object) }, null);
                if (objectInfo != null) {
                    return objectInfo.GetValue(indexableObject, new object[] { key });
                }
                return null;
            }

            private static object GetPropertyValue(object container, string propertyName) {
                // This method handles one "segment" of a complex property expression

                // First, we try to evaluate the property based on its indexer
                object value = GetIndexedPropertyValue(container, propertyName);
                if (value != null) {
                    return value;
                }

                // If the indexer didn't return anything useful, continue...

                // If the container is a ViewDataDictionary then treat its Model property
                // as the container instead of the ViewDataDictionary itself.
                ViewDataDictionary vdd = container as ViewDataDictionary;
                if (vdd != null) {
                    container = vdd.Model;
                }

                // Second, we try to use PropertyDescriptors and treat the expression as a property name
                PropertyDescriptor descriptor = TypeDescriptor.GetProperties(container).Find(propertyName, true);
                if (descriptor == null) {
                    return null;
                }

                return descriptor.GetValue(container);
            }

            private struct ExpressionPair {
                public readonly string Left;
                public readonly string Right;

                public ExpressionPair(string left, string right) {
                    Left = left;
                    Right = right;
                }
            }
        }

        #region IEnumerable Members
        [SuppressMessage("Microsoft.Security", "CA2123:OverrideLinkDemandsShouldBeIdenticalToBase")]
        IEnumerator IEnumerable.GetEnumerator() {
            return ((IEnumerable)_innerDictionary).GetEnumerator();
        }
        #endregion

    }
}
