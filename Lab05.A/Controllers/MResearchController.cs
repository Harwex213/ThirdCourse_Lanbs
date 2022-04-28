using System.Web.Mvc;

namespace Lab05.A.Controllers
{
    public class MResearchController : Controller
    {
        public MResearchController()
        {
            ActionInvoker = new ControllerActionInvoker();
        }
        
        [Route("MResearch/M01/1")]
        [Route("MResearch/M01")]
        [Route("MResearch")]
        [Route("")]
        [Route("V2/MResearch/M01")]
        [Route("V3/MResearch/X/M01")]
        public string M01()
        {
            return "GET:M01";
        }
        
        [Route("V2")]
        [Route("V2/MResearch")]
        [Route("V2/MResearch/M02")]
        [Route("MResearch/M02")]
        [Route("V3/MResearch/{id:int}/M02")]
        public string M02()
        {
            return "GET:M02";
        }
        
        [Route("V3")]
        [Route("V3/MResearch/{id:int}")]
        [Route("V3/MResearch/{id:int}/M03")]
        public string M03()
        {
            return "GET:M03";
        }
        
        [Route("MResearch/MMX")]
        public string MMX()
        {
            return "GET:MMX";
        }
    }
}