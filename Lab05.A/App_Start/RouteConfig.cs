using System.Web.Mvc;
using System.Web.Routing;
using Lab05.A.Handlers;

namespace Lab05.A
{
    public class RouteConfig
    {
        public static void RegisterRoutes(RouteCollection routes)
        {
            routes.MapRoute(
                name: "CResearch",
                url: "CResearch/{action}",
                defaults: new { controller = "CResearch", action = "C01" }
            );
            
            routes.Add(new Route("{controller}/{action}/{id}", 
                new RouteValueDictionary
                {
                    { "controller", "MResearch" },
                    { "action", "M01" },
                    { "id", UrlParameter.Optional }
                }, 
                new RouteValueDictionary { { "controller", "MResearch"} }, 
                new RouteValueDictionary { { "version", MResearchVersion.Default } }, 
                new MResearchRouteHandler())
            );

            routes.Add(new Route("V2/{controller}/{action}", 
                new RouteValueDictionary
                {
                    { "controller", "MResearch"},
                    { "action", "M02" },
                }, 
                new RouteValueDictionary { { "controller", "MResearch" } }, 
                new RouteValueDictionary { { "version", MResearchVersion.V2 } }, 
                new MResearchRouteHandler())
            );
            
            routes.Add(new Route("V3/{*action}",
                new RouteValueDictionary { { "action", @"MResearch/X"} },
                null, 
                new RouteValueDictionary { { "version", MResearchVersion.V3 } },
                new MResearchRouteHandler())
            );
        }
    }
}