using System;
using System.Text.RegularExpressions;
using System.Web;
using System.Web.Mvc;
using System.Web.Routing;
using Lab05.A.Controllers;

namespace Lab05.A.Handlers
{
    public interface IProcessRequestHandler
    {
        string ResolveActionName(RouteData routeData);
        string[] ValidActions { get; }
    }
    
    public class HttpHandler : IHttpHandler
    {
        public bool IsReusable => false;
        private readonly MResearchController _controller;
        private readonly IProcessRequestHandler _handler;

        public HttpHandler(string version)
        {
            _controller = new MResearchController();
            switch (version)
            {
                case nameof(MResearchVersion.V2):
                    _handler = new V2ProcessRequestHandler();
                    break;
                case nameof(MResearchVersion.V3):
                    _handler = new V3ProcessRequestHandler();
                    break;
                default:
                    _handler = new DefaultProcessRequestHandler();
                    break;
            }
        }
        
        public void ProcessRequest(HttpContext context)
        {
            try
            {
                var routeData = context.Request.RequestContext.RouteData;

                var action = _handler.ResolveActionName(routeData);
                var pattern = ConstructActionsRegex(_handler.ValidActions);
                if (Regex.IsMatch(action, pattern) == false)
                {
                    throw new Exception();
                }
                
                _controller.ControllerContext = new ControllerContext(context.Request.RequestContext, _controller);
                if (_controller.ActionInvoker.InvokeAction(_controller.ControllerContext, action) == false)
                {
                    throw new Exception();
                }
            }
            catch (Exception)
            {
                context.Response.StatusCode = 404;
            }
        }

        private string ConstructActionsRegex(string[] methods)
        {
            return "^(" + string.Join("|", methods) + ")$";
        }
    }
    
    public class DefaultProcessRequestHandler : IProcessRequestHandler
    {
        public string[] ValidActions => new[]
        {
            nameof(MResearchController.M01),
            nameof(MResearchController.M02),
            nameof(MResearchController.MXX)
        };

        private const string IdPattern = @"^1?$";
        
        public string ResolveActionName(RouteData routeData)
        {
            var id = routeData.Values[RouteConstants.IdName];
            var action = routeData.Values[RouteConstants.ActionName].ToString();
            
            if (id != UrlParameter.Optional && action != nameof(MResearchController.M01))
            {
                throw new Exception();
            }
            if (Regex.IsMatch(id.ToString(), IdPattern) == false)
            {
                throw new Exception();
            }

            return routeData.Values[RouteConstants.ActionName].ToString();
        }
    }
        
    public class V2ProcessRequestHandler : IProcessRequestHandler
    {
        public string[] ValidActions => new[]
        {
            nameof(MResearchController.M01),
            nameof(MResearchController.M02)
        };
        
        public string ResolveActionName(RouteData routeData)
        {
            return routeData.Values[RouteConstants.ActionName].ToString();
        }
    }
        
    public class V3ProcessRequestHandler : IProcessRequestHandler
    {
        public string[] ValidActions => new[]
        {
            nameof(MResearchController.M01),
            nameof(MResearchController.M02),
            nameof(MResearchController.M03)
        };

        private const string ActionPattern = @"^MResearch/(X|X/\w+)$";
        
        public string ResolveActionName(RouteData routeData)
        {
            const int actionNameIndex = 2;
            var action = routeData.Values[RouteConstants.ActionName].ToString();
            
            if (Regex.IsMatch(action, ActionPattern) == false)
            {
                throw new Exception();
            }
            
            var routes = action.Split('/');
            return routes.Length == 2 ? nameof(MResearchController.M03) : routes[actionNameIndex];
        }
    }
}