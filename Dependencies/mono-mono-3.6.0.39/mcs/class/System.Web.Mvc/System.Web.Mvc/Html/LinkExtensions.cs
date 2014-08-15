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

namespace System.Web.Mvc.Html {
    using System;
    using System.Collections.Generic;
    using System.Web.Mvc.Resources;
    using System.Web.Routing;

    public static class LinkExtensions {
        public static string ActionLink(this HtmlHelper htmlHelper, string linkText, string actionName) {
            return ActionLink(htmlHelper, linkText, actionName, null /* controllerName */, new RouteValueDictionary(), new RouteValueDictionary());
        }

        public static string ActionLink(this HtmlHelper htmlHelper, string linkText, string actionName, object routeValues) {
            return ActionLink(htmlHelper, linkText, actionName, null /* controllerName */, new RouteValueDictionary(routeValues), new RouteValueDictionary());
        }

        public static string ActionLink(this HtmlHelper htmlHelper, string linkText, string actionName, object routeValues, object htmlAttributes) {
            return ActionLink(htmlHelper, linkText, actionName, null /* controllerName */, new RouteValueDictionary(routeValues), new RouteValueDictionary(htmlAttributes));
        }

        public static string ActionLink(this HtmlHelper htmlHelper, string linkText, string actionName, RouteValueDictionary routeValues) {
            return ActionLink(htmlHelper, linkText, actionName, null /* controllerName */, routeValues, new RouteValueDictionary());
        }

        public static string ActionLink(this HtmlHelper htmlHelper, string linkText, string actionName, RouteValueDictionary routeValues, IDictionary<string, object> htmlAttributes) {
            return ActionLink(htmlHelper, linkText, actionName, null /* controllerName */, routeValues, htmlAttributes);
        }

        public static string ActionLink(this HtmlHelper htmlHelper, string linkText, string actionName, string controllerName) {
            return ActionLink(htmlHelper, linkText, actionName, controllerName, new RouteValueDictionary(), new RouteValueDictionary());
        }

        public static string ActionLink(this HtmlHelper htmlHelper, string linkText, string actionName, string controllerName, object routeValues, object htmlAttributes) {
            return ActionLink(htmlHelper, linkText, actionName, controllerName, new RouteValueDictionary(routeValues), new RouteValueDictionary(htmlAttributes));
        }

        public static string ActionLink(this HtmlHelper htmlHelper, string linkText, string actionName, string controllerName, RouteValueDictionary routeValues, IDictionary<string, object> htmlAttributes) {
            if (String.IsNullOrEmpty(linkText)) {
                throw new ArgumentException(MvcResources.Common_NullOrEmpty, "linkText");
            }
            return HtmlHelper.GenerateLink(htmlHelper.ViewContext.RequestContext, htmlHelper.RouteCollection, linkText, null/* routeName */, actionName, controllerName, routeValues, htmlAttributes);
        }

        public static string ActionLink(this HtmlHelper htmlHelper, string linkText, string actionName, string controllerName, string protocol, string hostName, string fragment, object routeValues, object htmlAttributes) {
            return ActionLink(htmlHelper, linkText, actionName, controllerName, protocol, hostName, fragment, new RouteValueDictionary(routeValues), new RouteValueDictionary(htmlAttributes));
        }

        public static string ActionLink(this HtmlHelper htmlHelper, string linkText, string actionName, string controllerName, string protocol, string hostName, string fragment, RouteValueDictionary routeValues, IDictionary<string, object> htmlAttributes) {
            if (String.IsNullOrEmpty(linkText)) {
                throw new ArgumentException(MvcResources.Common_NullOrEmpty, "linkText");
            }
            return HtmlHelper.GenerateLink(htmlHelper.ViewContext.RequestContext, htmlHelper.RouteCollection, linkText, null /* routeName */, actionName, controllerName, protocol, hostName, fragment, routeValues, htmlAttributes);
        }

        public static string RouteLink(this HtmlHelper htmlHelper, string linkText, object routeValues) {
            return RouteLink(htmlHelper, linkText, new RouteValueDictionary(routeValues));
        }

        public static string RouteLink(this HtmlHelper htmlHelper, string linkText, RouteValueDictionary routeValues) {
            return RouteLink(htmlHelper, linkText, routeValues, new RouteValueDictionary());
        }

        public static string RouteLink(this HtmlHelper htmlHelper, string linkText, string routeName, object routeValues) {
            return RouteLink(htmlHelper, linkText, routeName, new RouteValueDictionary(routeValues));
        }

        public static string RouteLink(this HtmlHelper htmlHelper, string linkText, string routeName, RouteValueDictionary routeValues) {
            return RouteLink(htmlHelper, linkText, routeName, routeValues, new RouteValueDictionary());
        }

        public static string RouteLink(this HtmlHelper htmlHelper, string linkText, object routeValues, object htmlAttributes) {
            return RouteLink(htmlHelper, linkText, new RouteValueDictionary(routeValues), new RouteValueDictionary(htmlAttributes));
        }

        public static string RouteLink(this HtmlHelper htmlHelper, string linkText, RouteValueDictionary routeValues, IDictionary<string, object> htmlAttributes) {
            return RouteLink(htmlHelper, linkText, null /* routeName */, routeValues, htmlAttributes);
        }

        public static string RouteLink(this HtmlHelper htmlHelper, string linkText, string routeName, object routeValues, object htmlAttributes) {
            return RouteLink(htmlHelper, linkText, routeName, new RouteValueDictionary(routeValues), new RouteValueDictionary(htmlAttributes));
        }

        public static string RouteLink(this HtmlHelper htmlHelper, string linkText, string routeName, RouteValueDictionary routeValues, IDictionary<string, object> htmlAttributes) {
            if (String.IsNullOrEmpty(linkText)) {
                throw new ArgumentException(MvcResources.Common_NullOrEmpty, "linkText");
            }
            return HtmlHelper.GenerateRouteLink(htmlHelper.ViewContext.RequestContext, htmlHelper.RouteCollection, linkText, routeName, routeValues, htmlAttributes);
        }

        public static string RouteLink(this HtmlHelper htmlHelper, string linkText, string routeName, string protocol, string hostName, string fragment, object routeValues, object htmlAttributes) {
            return RouteLink(htmlHelper, linkText, routeName, protocol, hostName, fragment, new RouteValueDictionary(routeValues), new RouteValueDictionary(htmlAttributes));
        }

        public static string RouteLink(this HtmlHelper htmlHelper, string linkText, string routeName, string protocol, string hostName, string fragment, RouteValueDictionary routeValues, IDictionary<string, object> htmlAttributes) {
            if (String.IsNullOrEmpty(linkText)) {
                throw new ArgumentException(MvcResources.Common_NullOrEmpty, "linkText");
            }
            return HtmlHelper.GenerateRouteLink(htmlHelper.ViewContext.RequestContext, htmlHelper.RouteCollection, linkText, routeName, protocol, hostName, fragment, routeValues, htmlAttributes);
        }
    }
}
