using System.Web.Mvc;

namespace Lab02.Controllers
{
    public class ErrorController : Controller
    {
        public ActionResult NotFound()
        {
            ViewBag.Method = Request.HttpMethod;
            ViewBag.Uri = Request.Url?.AbsoluteUri.Substring(Request.Url.AbsoluteUri.IndexOf(';') + 1);
            return View();
        }

    }
}