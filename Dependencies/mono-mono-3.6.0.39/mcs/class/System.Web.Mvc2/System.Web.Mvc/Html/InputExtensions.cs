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
    using System.Data.Linq;
    using System.Diagnostics.CodeAnalysis;
    using System.Globalization;
    using System.Linq.Expressions;
    using System.Text;
    using System.Web.Mvc.Resources;
    using System.Web.Routing;

    public static class InputExtensions {
        // CheckBox

        public static MvcHtmlString CheckBox(this HtmlHelper htmlHelper, string name) {
            return CheckBox(htmlHelper, name, (object)null /* htmlAttributes */);
        }

        public static MvcHtmlString CheckBox(this HtmlHelper htmlHelper, string name, bool isChecked) {
            return CheckBox(htmlHelper, name, isChecked, (object)null /* htmlAttributes */);
        }

        public static MvcHtmlString CheckBox(this HtmlHelper htmlHelper, string name, bool isChecked, object htmlAttributes) {
            return CheckBox(htmlHelper, name, isChecked, new RouteValueDictionary(htmlAttributes));
        }

        public static MvcHtmlString CheckBox(this HtmlHelper htmlHelper, string name, object htmlAttributes) {
            return CheckBox(htmlHelper, name, new RouteValueDictionary(htmlAttributes));
        }

        public static MvcHtmlString CheckBox(this HtmlHelper htmlHelper, string name, IDictionary<string, object> htmlAttributes) {
            return CheckBoxHelper(htmlHelper, name, null /* isChecked */, htmlAttributes);
        }

        public static MvcHtmlString CheckBox(this HtmlHelper htmlHelper, string name, bool isChecked, IDictionary<string, object> htmlAttributes) {
            return CheckBoxHelper(htmlHelper, name, isChecked, htmlAttributes);
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString CheckBoxFor<TModel>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, bool>> expression) {
            return CheckBoxFor(htmlHelper, expression, null /* htmlAttributes */);
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString CheckBoxFor<TModel>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, bool>> expression, object htmlAttributes) {
            return CheckBoxFor(htmlHelper, expression, new RouteValueDictionary(htmlAttributes));
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString CheckBoxFor<TModel>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, bool>> expression, IDictionary<string, object> htmlAttributes) {
            if (expression == null) {
                throw new ArgumentNullException("expression");
            }

            ModelMetadata metadata = ModelMetadata.FromLambdaExpression(expression, htmlHelper.ViewData);
            bool? isChecked = null;
            if (metadata.Model != null) {
                bool modelChecked;
                if (Boolean.TryParse(metadata.Model.ToString(), out modelChecked)) {
                    isChecked = modelChecked;
                }
            }

            return CheckBoxHelper(htmlHelper, ExpressionHelper.GetExpressionText(expression), isChecked, htmlAttributes);
        }

        private static MvcHtmlString CheckBoxHelper(HtmlHelper htmlHelper, string name, bool? isChecked, IDictionary<string, object> htmlAttributes) {
            RouteValueDictionary attributes =
                htmlAttributes == null ? new RouteValueDictionary()
                                       : new RouteValueDictionary(htmlAttributes);

            bool explicitValue = isChecked.HasValue;
            if (explicitValue) {
                attributes.Remove("checked");    // Explicit value must override dictionary
            }

            return InputHelper(htmlHelper, InputType.CheckBox, name, "true", !explicitValue /* useViewData */, isChecked ?? false, true /* setId */, false /* isExplicitValue */, attributes);
        }

        // Hidden

        public static MvcHtmlString Hidden(this HtmlHelper htmlHelper, string name) {
            return Hidden(htmlHelper, name, null /* value */, null /* htmlAttributes */);
        }

        public static MvcHtmlString Hidden(this HtmlHelper htmlHelper, string name, object value) {
            return Hidden(htmlHelper, name, value, null /* hmtlAttributes */);
        }

        public static MvcHtmlString Hidden(this HtmlHelper htmlHelper, string name, object value, object htmlAttributes) {
            return Hidden(htmlHelper, name, value, new RouteValueDictionary(htmlAttributes));
        }

        public static MvcHtmlString Hidden(this HtmlHelper htmlHelper, string name, object value, IDictionary<string, object> htmlAttributes) {
            return HiddenHelper(htmlHelper,
                                value,
                                value == null /* useViewData */,
                                name,
                                htmlAttributes);
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString HiddenFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression) {
            return HiddenFor(htmlHelper, expression, (IDictionary<string, object>)null);
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString HiddenFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression, object htmlAttributes) {
            return HiddenFor(htmlHelper, expression, new RouteValueDictionary(htmlAttributes));
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString HiddenFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression, IDictionary<string, object> htmlAttributes) {
            return HiddenHelper(htmlHelper,
                                ModelMetadata.FromLambdaExpression(expression, htmlHelper.ViewData).Model,
                                false,
                                ExpressionHelper.GetExpressionText(expression),
                                htmlAttributes);
        }

        private static MvcHtmlString HiddenHelper(HtmlHelper htmlHelper, object value, bool useViewData, string expression, IDictionary<string, object> htmlAttributes) {
            Binary binaryValue = value as Binary;
            if (binaryValue != null) {
                value = binaryValue.ToArray();
            }

            byte[] byteArrayValue = value as byte[];
            if (byteArrayValue != null) {
                value = Convert.ToBase64String(byteArrayValue);
            }

            return InputHelper(htmlHelper, InputType.Hidden, expression, value, useViewData, false /* isChecked */, true /* setId */, true /* isExplicitValue */, htmlAttributes);
        }

        // Password

        public static MvcHtmlString Password(this HtmlHelper htmlHelper, string name) {
            return Password(htmlHelper, name, null /* value */);
        }

        public static MvcHtmlString Password(this HtmlHelper htmlHelper, string name, object value) {
            return Password(htmlHelper, name, value, null /* htmlAttributes */);
        }

        public static MvcHtmlString Password(this HtmlHelper htmlHelper, string name, object value, object htmlAttributes) {
            return Password(htmlHelper, name, value, new RouteValueDictionary(htmlAttributes));
        }

        public static MvcHtmlString Password(this HtmlHelper htmlHelper, string name, object value, IDictionary<string, object> htmlAttributes) {
            return PasswordHelper(htmlHelper, name, value, htmlAttributes);
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString PasswordFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression) {
            return PasswordFor(htmlHelper, expression, null /* htmlAttributes */);
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString PasswordFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression, object htmlAttributes) {
            return PasswordFor(htmlHelper, expression, new RouteValueDictionary(htmlAttributes));
        }

        [SuppressMessage("Microsoft.Design", "CA1011:ConsiderPassingBaseTypesAsParameters", Justification = "Users cannot use anonymous methods with the LambdaExpression type")]
        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString PasswordFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression, IDictionary<string, object> htmlAttributes) {
            if (expression == null) {
                throw new ArgumentNullException("expression");
            }

            return PasswordHelper(htmlHelper,
                                  ExpressionHelper.GetExpressionText(expression),
                                  null /* value */,
                                  htmlAttributes);
        }

        private static MvcHtmlString PasswordHelper(HtmlHelper htmlHelper, string name, object value, IDictionary<string, object> htmlAttributes) {
            return InputHelper(htmlHelper, InputType.Password, name, value, false /* useViewData */, false /* isChecked */, true /* setId */, true /* isExplicitValue */, htmlAttributes);
        }

        // RadioButton

        public static MvcHtmlString RadioButton(this HtmlHelper htmlHelper, string name, object value) {
            return RadioButton(htmlHelper, name, value, (object)null /* htmlAttributes */);
        }

        public static MvcHtmlString RadioButton(this HtmlHelper htmlHelper, string name, object value, object htmlAttributes) {
            return RadioButton(htmlHelper, name, value, new RouteValueDictionary(htmlAttributes));
        }

        public static MvcHtmlString RadioButton(this HtmlHelper htmlHelper, string name, object value, IDictionary<string, object> htmlAttributes) {
            // Determine whether or not to render the checked attribute based on the contents of ViewData.
            string valueString = Convert.ToString(value, CultureInfo.CurrentCulture);
            bool isChecked = (!String.IsNullOrEmpty(name)) && (String.Equals(htmlHelper.EvalString(name), valueString, StringComparison.OrdinalIgnoreCase));
            // checked attributes is implicit, so we need to ensure that the dictionary takes precedence.
            RouteValueDictionary attributes = htmlAttributes == null ? new RouteValueDictionary() : new RouteValueDictionary(htmlAttributes);
            if (attributes.ContainsKey("checked")) {
                return InputHelper(htmlHelper, InputType.Radio, name, value, false, false, true, true /* isExplicitValue */, attributes);
            }

            return RadioButton(htmlHelper, name, value, isChecked, htmlAttributes);
        }

        public static MvcHtmlString RadioButton(this HtmlHelper htmlHelper, string name, object value, bool isChecked) {
            return RadioButton(htmlHelper, name, value, isChecked, (object)null /* htmlAttributes */);
        }

        public static MvcHtmlString RadioButton(this HtmlHelper htmlHelper, string name, object value, bool isChecked, object htmlAttributes) {
            return RadioButton(htmlHelper, name, value, isChecked, new RouteValueDictionary(htmlAttributes));
        }

        public static MvcHtmlString RadioButton(this HtmlHelper htmlHelper, string name, object value, bool isChecked, IDictionary<string, object> htmlAttributes) {
            if (value == null) {
                throw new ArgumentNullException("value");
            }
            // checked attribute is an explicit parameter so it takes precedence.
            RouteValueDictionary attributes = htmlAttributes == null ? new RouteValueDictionary() : new RouteValueDictionary(htmlAttributes);
            attributes.Remove("checked");
            return InputHelper(htmlHelper, InputType.Radio, name, value, false, isChecked, true, true /* isExplicitValue */, attributes);
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString RadioButtonFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression, object value) {
            return RadioButtonFor(htmlHelper, expression, value, null /* htmlAttributes */);
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString RadioButtonFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression, object value, object htmlAttributes) {
            return RadioButtonFor(htmlHelper, expression, value, new RouteValueDictionary(htmlAttributes));
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString RadioButtonFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression, object value, IDictionary<string, object> htmlAttributes) {
            return RadioButtonHelper(htmlHelper,
                                     ModelMetadata.FromLambdaExpression(expression, htmlHelper.ViewData).Model,
                                     ExpressionHelper.GetExpressionText(expression),
                                     value,
                                     null /* isChecked */,
                                     htmlAttributes);
        }

        private static MvcHtmlString RadioButtonHelper(HtmlHelper htmlHelper, object model, string name, object value, bool? isChecked, IDictionary<string, object> htmlAttributes) {
            if (value == null) {
                throw new ArgumentNullException("value");
            }

            RouteValueDictionary attributes =
                htmlAttributes == null ? new RouteValueDictionary()
                                       : new RouteValueDictionary(htmlAttributes);

            bool explicitValue = isChecked.HasValue;
            if (explicitValue) {
                attributes.Remove("checked");    // Explicit value must override dictionary
            }
            else {
                string valueString = Convert.ToString(value, CultureInfo.CurrentCulture);
                isChecked = model != null &&
                            !String.IsNullOrEmpty(name) &&
                            String.Equals(model.ToString(), valueString, StringComparison.OrdinalIgnoreCase);
            }

            return InputHelper(htmlHelper, InputType.Radio, name, value, false /* useViewData */, isChecked ?? false, true /* setId */, true /* isExplicitValue */, attributes);
        }

        // TextBox

        public static MvcHtmlString TextBox(this HtmlHelper htmlHelper, string name) {
            return TextBox(htmlHelper, name, null /* value */);
        }

        public static MvcHtmlString TextBox(this HtmlHelper htmlHelper, string name, object value) {
            return TextBox(htmlHelper, name, value, (object)null /* htmlAttributes */);
        }

        public static MvcHtmlString TextBox(this HtmlHelper htmlHelper, string name, object value, object htmlAttributes) {
            return TextBox(htmlHelper, name, value, new RouteValueDictionary(htmlAttributes));
        }

        public static MvcHtmlString TextBox(this HtmlHelper htmlHelper, string name, object value, IDictionary<string, object> htmlAttributes) {
            return InputHelper(htmlHelper, InputType.Text, name, value, (value == null) /* useViewData */, false /* isChecked */, true /* setId */, true /* isExplicitValue */, htmlAttributes);
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString TextBoxFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression) {
            return htmlHelper.TextBoxFor(expression, (IDictionary<string, object>)null);
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString TextBoxFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression, object htmlAttributes) {
            return htmlHelper.TextBoxFor(expression, new RouteValueDictionary(htmlAttributes));
        }

        [SuppressMessage("Microsoft.Design", "CA1006:DoNotNestGenericTypesInMemberSignatures", Justification = "This is an appropriate nesting of generic types")]
        public static MvcHtmlString TextBoxFor<TModel, TProperty>(this HtmlHelper<TModel> htmlHelper, Expression<Func<TModel, TProperty>> expression, IDictionary<string, object> htmlAttributes) {
            return TextBoxHelper(htmlHelper,
                                 ModelMetadata.FromLambdaExpression(expression, htmlHelper.ViewData).Model,
                                 ExpressionHelper.GetExpressionText(expression),
                                 htmlAttributes);
        }

        private static MvcHtmlString TextBoxHelper(this HtmlHelper htmlHelper, object model, string expression, IDictionary<string, object> htmlAttributes) {
            return InputHelper(htmlHelper, InputType.Text, expression, model, false /* useViewData */, false /* isChecked */, true /* setId */, true /* isExplicitValue */, htmlAttributes);
        }

        // Helper methods

        private static MvcHtmlString InputHelper(HtmlHelper htmlHelper, InputType inputType, string name, object value, bool useViewData, bool isChecked, bool setId, bool isExplicitValue, IDictionary<string, object> htmlAttributes) {
            name = htmlHelper.ViewContext.ViewData.TemplateInfo.GetFullHtmlFieldName(name);
            if (String.IsNullOrEmpty(name)) {
                throw new ArgumentException(MvcResources.Common_NullOrEmpty, "name");
            }

            TagBuilder tagBuilder = new TagBuilder("input");
            tagBuilder.MergeAttributes(htmlAttributes);
            tagBuilder.MergeAttribute("type", HtmlHelper.GetInputTypeString(inputType));
            tagBuilder.MergeAttribute("name", name, true);

            string valueParameter = Convert.ToString(value, CultureInfo.CurrentCulture);
            bool usedModelState = false;

            switch (inputType) {
                case InputType.CheckBox:
                    bool? modelStateWasChecked = htmlHelper.GetModelStateValue(name, typeof(bool)) as bool?;
                    if (modelStateWasChecked.HasValue) {
                        isChecked = modelStateWasChecked.Value;
                        usedModelState = true;
                    }
                    goto case InputType.Radio;
                case InputType.Radio:
                    if (!usedModelState) {
                        string modelStateValue = htmlHelper.GetModelStateValue(name, typeof(string)) as string;
                        if (modelStateValue != null) {
                            isChecked = String.Equals(modelStateValue, valueParameter, StringComparison.Ordinal);
                            usedModelState = true;
                        }
                    }
                    if (!usedModelState && useViewData) {
                        isChecked = htmlHelper.EvalBoolean(name);
                    }
                    if (isChecked) {
                        tagBuilder.MergeAttribute("checked", "checked");
                    }
                    tagBuilder.MergeAttribute("value", valueParameter, isExplicitValue);
                    break;
                case InputType.Password:
                    if (value != null) {
                        tagBuilder.MergeAttribute("value", valueParameter, isExplicitValue);
                    }
                    break;
                default:
                    string attemptedValue = (string)htmlHelper.GetModelStateValue(name, typeof(string));
                    tagBuilder.MergeAttribute("value", attemptedValue ?? ((useViewData) ? htmlHelper.EvalString(name) : valueParameter), isExplicitValue);
                    break;
            }

            if (setId) {
                tagBuilder.GenerateId(name);
            }

            // If there are any errors for a named field, we add the css attribute.
            ModelState modelState;
            if (htmlHelper.ViewData.ModelState.TryGetValue(name, out modelState)) {
                if (modelState.Errors.Count > 0) {
                    tagBuilder.AddCssClass(HtmlHelper.ValidationInputCssClassName);
                }
            }

            if (inputType == InputType.CheckBox) {
                // Render an additional <input type="hidden".../> for checkboxes. This
                // addresses scenarios where unchecked checkboxes are not sent in the request.
                // Sending a hidden input makes it possible to know that the checkbox was present
                // on the page when the request was submitted.
                StringBuilder inputItemBuilder = new StringBuilder();
                inputItemBuilder.Append(tagBuilder.ToString(TagRenderMode.SelfClosing));

                TagBuilder hiddenInput = new TagBuilder("input");
                hiddenInput.MergeAttribute("type", HtmlHelper.GetInputTypeString(InputType.Hidden));
                hiddenInput.MergeAttribute("name", name);
                hiddenInput.MergeAttribute("value", "false");
                inputItemBuilder.Append(hiddenInput.ToString(TagRenderMode.SelfClosing));
                return MvcHtmlString.Create(inputItemBuilder.ToString());
            }

            return tagBuilder.ToMvcHtmlString(TagRenderMode.SelfClosing);
        }
    }
}
