using System;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Routing;
using Lab05.A.Controllers;

namespace Lab05.A.Handlers
{
    public class MResearchRouteHandler : IRouteHandler
    {
        public IHttpHandler GetHttpHandler(RequestContext requestContext)
        {
            return new TestHttpHandler(requestContext.RouteData);
        }
    }

    public class TestHttpHandler : IHttpHandler
    {
        public bool IsReusable => false;
        private readonly RouteData _routeData;
        private readonly MResearchController _controller;

        public TestHttpHandler(RouteData routeData)
        {
            _routeData = routeData;
            _controller = new MResearchController();
        }
        
        public void ProcessRequest(HttpContext context)
        {
            try
            {
                // if (_routeData.DataTokens.ContainsKey("default") &&
                //     _routeData.Values["data"] is string && _routeData.Values["action"].ToString() == "M02" )
                // {
                //     context.Response.Redirect("MResearch/MMX");
                // }
                
                var action = _routeData.Values["action"].ToString();
                _controller.ControllerContext = new ControllerContext(context.Request.RequestContext, _controller);
                _controller.ActionInvoker.InvokeAction(_controller.ControllerContext, action);
            }
            catch (Exception)
            {
                context.Response.Redirect("MResearch/MMX");
            }
        }
    }
}