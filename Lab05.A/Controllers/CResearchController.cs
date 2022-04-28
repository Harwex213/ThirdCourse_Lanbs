using System.IO;
using System.Linq;
using System.Web.Mvc;

namespace Lab05.A.Controllers
{
    public class CResearchController : Controller
    {
        [AcceptVerbs("POST", "GET")]
        public JsonResult C01()
        {
            return Json(new
            {
                Action = "C01",
                Method = Request.HttpMethod,
                QueryParams = Request.QueryString.AllKeys.ToDictionary(key => key, key => Request.QueryString[key]),
                Uri = Request.Url?.AbsoluteUri,
                Headers = Request.Headers.AllKeys.ToDictionary(key => key, key => Request.Headers[key]),
                Body = new StreamReader(Request.InputStream).ReadToEnd().ToString(),
            }, JsonRequestBehavior.AllowGet);
        }

        [AcceptVerbs("POST", "GET")]
        public JsonResult C02()
        {
            return Json(new
            {
                Action = "C02",
                ResponseStatus = Response.StatusCode,
                RequestHeaders = Request.Headers.AllKeys.ToDictionary(key => key, key => Request.Headers[key]),
                ResponseHeaders = Response.Headers.AllKeys.ToDictionary(key => key, key => Response.Headers[key]),
                Body = new StreamReader(Request.InputStream).ReadToEnd().ToString()
            }, JsonRequestBehavior.AllowGet);
        }
    }
}