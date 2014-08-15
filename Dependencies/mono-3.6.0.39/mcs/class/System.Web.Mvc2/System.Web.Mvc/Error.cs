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
    using System.Globalization;
    using System.Web.Mvc.Async;
    using System.Web.Mvc.Resources;

    internal static class Error {

        public static InvalidOperationException AsyncActionMethodSelector_CouldNotFindMethod(string methodName, Type controllerType) {
            string message = String.Format(CultureInfo.CurrentUICulture, MvcResources.AsyncActionMethodSelector_CouldNotFindMethod,
                methodName, controllerType);
            return new InvalidOperationException(message);
        }

        public static InvalidOperationException AsyncCommon_AsyncResultAlreadyConsumed() {
            return new InvalidOperationException(MvcResources.AsyncCommon_AsyncResultAlreadyConsumed);
        }

        public static InvalidOperationException AsyncCommon_ControllerMustImplementIAsyncManagerContainer(Type actualControllerType) {
            string message = String.Format(CultureInfo.CurrentUICulture, MvcResources.AsyncCommon_ControllerMustImplementIAsyncManagerContainer,
                actualControllerType);
            return new InvalidOperationException(message);
        }

        public static ArgumentException AsyncCommon_InvalidAsyncResult(string parameterName) {
            return new ArgumentException(MvcResources.AsyncCommon_InvalidAsyncResult, parameterName);
        }

        public static ArgumentOutOfRangeException AsyncCommon_InvalidTimeout(string parameterName) {
            return new ArgumentOutOfRangeException(parameterName, MvcResources.AsyncCommon_InvalidTimeout);
        }

        public static InvalidOperationException ReflectedAsyncActionDescriptor_CannotExecuteSynchronously(string actionName) {
            string message = String.Format(CultureInfo.CurrentUICulture, MvcResources.ReflectedAsyncActionDescriptor_CannotExecuteSynchronously,
                actionName);
            return new InvalidOperationException(message);
        }

        public static InvalidOperationException ChildActionOnlyAttribute_MustBeInChildRequest(ActionDescriptor actionDescriptor) {
            string message = String.Format(CultureInfo.CurrentUICulture, MvcResources.ChildActionOnlyAttribute_MustBeInChildRequest,
                actionDescriptor.ActionName);
            return new InvalidOperationException(message);
        }

        public static ArgumentException ParameterCannotBeNullOrEmpty(string parameterName) {
            return new ArgumentException(MvcResources.Common_NullOrEmpty, parameterName);
        }

        public static InvalidOperationException PropertyCannotBeNullOrEmpty(string propertyName) {
            string message = String.Format(CultureInfo.CurrentUICulture, MvcResources.Common_PropertyCannotBeNullOrEmpty,
                propertyName);
            return new InvalidOperationException(message);
        }

        public static SynchronousOperationException SynchronizationContextUtil_ExceptionThrown(Exception innerException) {
            return new SynchronousOperationException(MvcResources.SynchronizationContextUtil_ExceptionThrown, innerException);
        }

        public static InvalidOperationException ViewDataDictionary_WrongTModelType(Type valueType, Type modelType) {
            string message = String.Format(CultureInfo.CurrentUICulture, MvcResources.ViewDataDictionary_WrongTModelType,
                valueType, modelType);
            return new InvalidOperationException(message);
        }

        public static InvalidOperationException ViewDataDictionary_ModelCannotBeNull(Type modelType) {
            string message = String.Format(CultureInfo.CurrentUICulture, MvcResources.ViewDataDictionary_ModelCannotBeNull,
                modelType);
            return new InvalidOperationException(message);
        }

    }
}
