﻿namespace System.Web.Mvc {
    using System.Diagnostics.CodeAnalysis;

    public abstract class WebViewPage<TModel> : WebViewPage {
        private ViewDataDictionary<TModel> _viewData;

        public new AjaxHelper<TModel> Ajax {
            get;
            set;
        }

        public new HtmlHelper<TModel> Html {
            get;
            set;
        }

        public new TModel Model {
            get {
                return ViewData.Model;
            }
        }

        [SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly", Justification = "This is the mechanism by which the ViewPage gets its ViewDataDictionary object.")]
        public new ViewDataDictionary<TModel> ViewData {
            get {
                if (_viewData == null) {
                    SetViewData(new ViewDataDictionary<TModel>());
                }
                return _viewData;
            }
            set {
                SetViewData(value);
            }
        }

        public override void InitHelpers() {
            base.InitHelpers();

            Ajax = new AjaxHelper<TModel>(ViewContext, this);
            Html = new HtmlHelper<TModel>(ViewContext, this);
        }

        protected override void SetViewData(ViewDataDictionary viewData) {
            _viewData = new ViewDataDictionary<TModel>(viewData);

            base.SetViewData(_viewData);
        }
    }
}
