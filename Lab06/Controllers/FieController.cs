using System.Web.Mvc;

namespace Lab06.Controllers
{
    public class FieController : Controller
    {
        [Authorize(Roles = "Guest, Employer")]
        public ActionResult Index()
        {
            return View();
        }
        
        [Authorize(Roles = "Employer")]
        public ActionResult FIE_TM()
        {
            return View();
        }
        
        [Authorize(Roles = "Employer")]
        public ActionResult FIE_UR()
        {
            return View();
        }
        
        [Authorize(Roles = "Employer")]
        public ActionResult FIE_UP()
        {
            return View();
        }
    }
}