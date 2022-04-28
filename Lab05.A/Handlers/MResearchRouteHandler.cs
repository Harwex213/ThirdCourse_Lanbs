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
            return new HttpHandler(requestContext.RouteData.DataTokens["version"].ToString());
        }
    }
}