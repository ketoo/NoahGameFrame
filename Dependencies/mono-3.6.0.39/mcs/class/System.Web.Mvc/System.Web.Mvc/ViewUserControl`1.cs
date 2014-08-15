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
    using System.Diagnostics.CodeAnalysis;

    public class ViewUserControl<TModel> : ViewUserControl where TModel : class {
        private AjaxHelper<TModel> _ajaxHelper;
        private HtmlHelper<TModel> _htmlHelper;
        private ViewDataDictionary<TModel> _viewData;

        public new AjaxHelper<TModel> Ajax {
            get {
                if (_ajaxHelper == null) {
                    _ajaxHelper = new AjaxHelper<TModel>(ViewContext, this);
                }
                return _ajaxHelper;
            }
        }

        public new HtmlHelper<TModel> Html {
            get {
                if (_htmlHelper == null) {
                    _htmlHelper = new HtmlHelper<TModel>(ViewContext, this);
                }
                return _htmlHelper;
            }
        }

        public new TModel Model {
            get {
                return ViewData.Model;
            }            
        }

        [SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public new ViewDataDictionary<TModel> ViewData {
            get {
                EnsureViewData();
                return _viewData;
            }
            set {
                SetViewData(value);
            }
        }

        protected override void SetViewData(ViewDataDictionary viewData) {
            _viewData = new ViewDataDictionary<TModel>(viewData);

            base.SetViewData(_viewData);
        }
    }
}
