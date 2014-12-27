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
    using System.Globalization;
    using System.Web.Mvc.Resources;

    public class ModelBinderDictionary : IDictionary<Type, IModelBinder> {

        private IModelBinder _defaultBinder;
        private readonly Dictionary<Type, IModelBinder> _innerDictionary = new Dictionary<Type, IModelBinder>();

        public int Count {
            get {
                return _innerDictionary.Count;
            }
        }

        public IModelBinder DefaultBinder {
            get {
                if (_defaultBinder == null) {
                    _defaultBinder = new DefaultModelBinder();
                }
                return _defaultBinder;
            }
            set {
                _defaultBinder = value;
            }
        }

        public bool IsReadOnly {
            get {
                return ((IDictionary<Type, IModelBinder>)_innerDictionary).IsReadOnly;
            }
        }

        public ICollection<Type> Keys {
            get {
                return _innerDictionary.Keys;
            }
        }

        public IModelBinder this[Type key] {
            get {
                IModelBinder binder;
                _innerDictionary.TryGetValue(key, out binder);
                return binder;
            }
            set {
                _innerDictionary[key] = value;
            }
        }

        public ICollection<IModelBinder> Values {
            get {
                return _innerDictionary.Values;
            }
        }

        public void Add(KeyValuePair<Type, IModelBinder> item) {
            ((IDictionary<Type, IModelBinder>)_innerDictionary).Add(item);
        }

        public void Add(Type key, IModelBinder value) {
            _innerDictionary.Add(key, value);
        }

        public void Clear() {
            _innerDictionary.Clear();
        }

        public bool Contains(KeyValuePair<Type, IModelBinder> item) {
            return ((IDictionary<Type, IModelBinder>)_innerDictionary).Contains(item);
        }

        public bool ContainsKey(Type key) {
            return _innerDictionary.ContainsKey(key);
        }

        public void CopyTo(KeyValuePair<Type, IModelBinder>[] array, int arrayIndex) {
            ((IDictionary<Type, IModelBinder>)_innerDictionary).CopyTo(array, arrayIndex);
        }

        public IModelBinder GetBinder(Type modelType) {
            return GetBinder(modelType, true /* fallbackToDefault */);
        }

        public virtual IModelBinder GetBinder(Type modelType, bool fallbackToDefault) {
            if (modelType == null) {
                throw new ArgumentNullException("modelType");
            }

            return GetBinder(modelType, (fallbackToDefault) ? DefaultBinder : null);
        }

        private IModelBinder GetBinder(Type modelType, IModelBinder fallbackBinder) {
            // Try to look up a binder for this type. We use this order of precedence:
            // 1. Binder registered in the global table
            // 2. Binder attribute defined on the type
            // 3. Supplied fallback binder

            IModelBinder binder;
            if (_innerDictionary.TryGetValue(modelType, out binder)) {
                return binder;
            }

            binder = ModelBinders.GetBinderFromAttributes(modelType,
                () => String.Format(CultureInfo.CurrentUICulture, MvcResources.ModelBinderDictionary_MultipleAttributes, modelType.FullName));

            return binder ?? fallbackBinder;
        }

        public IEnumerator<KeyValuePair<Type, IModelBinder>> GetEnumerator() {
            return _innerDictionary.GetEnumerator();
        }

        public bool Remove(KeyValuePair<Type, IModelBinder> item) {
            return ((IDictionary<Type, IModelBinder>)_innerDictionary).Remove(item);
        }

        public bool Remove(Type key) {
            return _innerDictionary.Remove(key);
        }

        public bool TryGetValue(Type key, out IModelBinder value) {
            return _innerDictionary.TryGetValue(key, out value);
        }

        #region IEnumerable Members
        IEnumerator IEnumerable.GetEnumerator() {
            return ((IEnumerable)_innerDictionary).GetEnumerator();
        }
        #endregion

    }
}
