using System.Web.Mvc;

namespace Lab06.Controllers
{
    [Authorize]
    public class BstuController : Controller
    {
        public ActionResult Index()
        {
            return View();
        }
        
        [Authorize(Roles = "Administrator")]
        public ActionResult Config()
        {
            return View();
        }
    }
}