using System.Web.Mvc;

namespace Lab06.Controllers
{
    public class FitController : Controller
    {
        [Authorize(Roles = "Guest, Employer")]
        public ActionResult Index()
        {
            return View();
        }
        
        [Authorize(Roles = "Employer")]
        public ActionResult FIT_IS()
        {
            return View();
        }
        
        [Authorize(Roles = "Employer")]
        public ActionResult FIT_PI()
        {
            return View();
        }
        
        [Authorize(Roles = "Employer")]
        public ActionResult FIT_ID()
        {
            return View();
        }
    }
}