using System;
using System.Web.Mvc;

namespace Lab05.B.Controllers
{
    public class CHResearchController : Controller
    {
        private string _value = "hello from oleg. Now is ";

        [HttpGet, OutputCache(Duration = 5, Location = System.Web.UI.OutputCacheLocation.Any)]
        public string AD()
        {
            return _value + DateTime.Now;
        }

        [HttpPost, OutputCache(Duration = 7, VaryByParam = "x;y", Location = System.Web.UI.OutputCacheLocation.Server)]
        public string AP(string x, string y)
        {
            return _value + DateTime.Now;
        }
    }
}