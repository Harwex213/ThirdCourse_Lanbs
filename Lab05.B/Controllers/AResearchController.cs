using System;
using System.Web.Mvc;

namespace Lab05.B.Controllers
{
    public class AResearchController : Controller
    {
        [AA]
        public string AA()
        {
            Response.Write("<p>AA 2</p>");
            return "AA";
        }
        
        [AA]
        [AR]
        public string AR()
        {
            Response.Write("<p>AR 2</p>");
            return "AR";
        }
        
        [AE]
        public string AE()
        {
            Response.Write("<p>AE 2</p>");
            throw new Exception();
        }
    }
    
    public class AAAttribute : FilterAttribute, IActionFilter
    {
        public void OnActionExecuting(ActionExecutingContext filterContext) 
        {
            filterContext.HttpContext.Response.Write("<p>AA: OnActionExecuting</p>");
        }

        public void OnActionExecuted(ActionExecutedContext filterContext)
        {
            filterContext.HttpContext.Response.Write("<p>AA: OnActionExecuted</p>");
        }
    }

    
    public class ARAttribute : FilterAttribute, IResultFilter
    {

        public void OnResultExecuting(ResultExecutingContext filterContext)
        {
            filterContext.HttpContext.Response.Write("<p>AR: OnResultExecuting</p>");
        }
        
        public void OnResultExecuted(ResultExecutedContext filterContext)
        {
            filterContext.HttpContext.Response.Write("<p>AR: OnResultExecuted</p>");
        }
    }

    
    public class AEAttribute : FilterAttribute, IExceptionFilter
    {
        public void OnException(ExceptionContext filterContext)
        {
            filterContext.HttpContext.Response.Write("<p>AE: OnException</p>");
            filterContext.ExceptionHandled = true;
        }
    }

}