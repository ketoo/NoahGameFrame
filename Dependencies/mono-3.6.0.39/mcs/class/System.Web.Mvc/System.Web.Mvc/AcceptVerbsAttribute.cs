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
    using System.Collections.ObjectModel;
    using System.Diagnostics.CodeAnalysis;
    using System.Linq;
    using System.Reflection;
    using System.Web.Mvc.Resources;

    [SuppressMessage("Microsoft.Design", "CA1019:DefineAccessorsForAttributeArguments",
        Justification = "The accessor is exposed as an ICollection<string>.")]
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false, Inherited = true)]
    public sealed class AcceptVerbsAttribute : ActionMethodSelectorAttribute {

        public AcceptVerbsAttribute(HttpVerbs verbs)
            : this(EnumToArray(verbs)) {
        }

        public AcceptVerbsAttribute(params string[] verbs) {
            if (verbs == null || verbs.Length == 0) {
                throw new ArgumentException(MvcResources.Common_NullOrEmpty, "verbs");
            }

            Verbs = new ReadOnlyCollection<string>(verbs);
        }

        public ICollection<string> Verbs {
            get;
            private set;
        }

        private static void AddEntryToList(HttpVerbs verbs, HttpVerbs match, List<string> verbList, string entryText) {
            if ((verbs & match) != 0) {
                verbList.Add(entryText);
            }
        }

        internal static string[] EnumToArray(HttpVerbs verbs) {
            List<string> verbList = new List<string>();

            AddEntryToList(verbs, HttpVerbs.Get, verbList, "GET");
            AddEntryToList(verbs, HttpVerbs.Post, verbList, "POST");
            AddEntryToList(verbs, HttpVerbs.Put, verbList, "PUT");
            AddEntryToList(verbs, HttpVerbs.Delete, verbList, "DELETE");
            AddEntryToList(verbs, HttpVerbs.Head, verbList, "HEAD");

            return verbList.ToArray();
        }

        public override bool IsValidForRequest(ControllerContext controllerContext, MethodInfo methodInfo) {
            if (controllerContext == null) {
                throw new ArgumentNullException("controllerContext");
            }

            string incomingVerb = controllerContext.HttpContext.Request.HttpMethod;
            return Verbs.Contains(incomingVerb, StringComparer.OrdinalIgnoreCase);
        }
    }
}
