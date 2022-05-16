using System.Web.Mvc;

namespace Lab06.Controllers
{
    public class FltController : Controller
    {
        [Authorize(Roles = "Guest, Employer")]
        public ActionResult Index()
        {
            return View();
        }
        
        [Authorize(Roles = "Employer")]
        public ActionResult FLT_LU()
        {
            return View();
        }
        
        [Authorize(Roles = "Employer")]
        public ActionResult FLT_LV()
        {
            return View();
        }
        
        [Authorize(Roles = "Employer")]
        public ActionResult FLT_LZ()
        {
            return View();
        }
    }
}