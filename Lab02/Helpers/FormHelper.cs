using System.Linq;
using System.Web.Mvc;
using System.Web.Mvc.Html;
using Lab02.DTOs.Dict;

namespace Lab02.Helpers
{
    public static class FormHelper
    {
        public static MvcHtmlString RenderRecordControls(this HtmlHelper htmlHelper, SelectList operators)
        {
            const int nameControlIndex = 0;
            const int codeControlIndex = 1;
            const int numberControlIndex = 2;
            var controls = new TagBuilder[3];

            // input Name
            controls[nameControlIndex] = new TagBuilder("div")
            {
                Attributes =
                {
                    { "class", "mb-4" }
                }
            };
            var nameControlLabel = new TagBuilder("label")
            {
                Attributes =
                {
                    { "for", "Name" },
                    { "class", "form-label" },
                },
                InnerHtml = "Name"
            };
            var nameControlError = new TagBuilder("p")
            {
                Attributes =
                {
                    { "class", "text text-danger" }
                },
                InnerHtml = htmlHelper.ValidationMessage("Name").ToString()
            };
            var nameControl = htmlHelper.Editor("Name",
                new { htmlAttributes = new { @class="form-control" }});
            controls[nameControlIndex].InnerHtml = nameControlLabel.ToString() + nameControl + nameControlError;

            // input Code
            controls[codeControlIndex] = new TagBuilder("div")
            {
                Attributes =
                {
                    { "class", "mb-4" }
                }
            };
            var codeControlLabel = new TagBuilder("label")
            {
                Attributes =
                {
                    { "for", "Code" },
                    { "class", "form-label" },
                },
                InnerHtml = "Code"
            };
            var codeControlError = new TagBuilder("p")
            {
                Attributes =
                {
                    { "class", "text text-danger" }
                },
                InnerHtml = htmlHelper.ValidationMessage("Code").ToString()
            };
            var codeControl = htmlHelper.DropDownList("Code",
                operators,
                new { @class = "form-select" });
            controls[codeControlIndex].InnerHtml = codeControlLabel.ToString() + codeControl + codeControlError;

            // input Number
            controls[numberControlIndex] = new TagBuilder("div")
            {
                Attributes =
                {
                    { "class", "mb-4" }
                }
            };
            var numberControlLabel = new TagBuilder("label")
            {
                Attributes =
                {
                    { "for", "Number" },
                    { "class", "form-label" },
                },
                InnerHtml = "Number"
            };
            var numberControlError = new TagBuilder("p")
            {
                Attributes =
                {
                    { "class", "text text-danger" }
                },
                InnerHtml = htmlHelper.ValidationMessage("Number").ToString()
            };
            var numberControl = htmlHelper.Editor("Number",
                new { htmlAttributes = new { @class="form-control" }});
            controls[numberControlIndex].InnerHtml = numberControlLabel.ToString() + numberControl + numberControlError;

            return new MvcHtmlString(string.Join<TagBuilder>(" ", controls));
        }
    }
}