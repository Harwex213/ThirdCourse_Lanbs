using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Routing;
using Lab05.A.Handlers;

namespace Lab05.A
{
    public class RouteConfig
    {
        public static void RegisterRoutes(RouteCollection routes)
        {
            // routes.IgnoreRoute("{resource}.axd/{*pathInfo}");
            // routes.IgnoreRoute("{resource}.*");

            // routes.MapMvcAttributeRoutes();
            
            routes.MapRoute(
                name: "CResearch",
                url: "CResearch/{action}",
                defaults: new { controller = "CResearch", action = "C01" }
            );

            routes.Add(
                "Default",
                new Route("{controller}/{action}", 
                new RouteValueDictionary
                {
                    { "controller", "MResearch" },
                    { "action", "M01" }
                },
                new RouteValueDictionary
                {
                    { "controller", "MResearch" },
                    { "action", "^(M01|M02|MMX)$"}
                },
                new MResearchRouteHandler()));
            
            routes.Add(
                "Default/1",
                new Route("MResearch/{action}/1", 
                    new RouteValueDictionary { { "action", "M01" } },
                    new RouteValueDictionary { { "action", "M01"} },
                    new MResearchRouteHandler()));
            
            routes.Add(
                "V2",
                new Route("V2/{controller}/{action}", 
                new RouteValueDictionary
                {
                    { "controller", "MResearch" },
                    { "action", "M02" }
                },
                new RouteValueDictionary
                {
                    { "controller", "MResearch" },
                    { "action", "^(M01|M02)$"}
                },
                new MResearchRouteHandler()));
            
            routes.Add(
                "V3",
                new Route("V3/{controller}/{data}/{action}", 
                new RouteValueDictionary
                {
                    { "controller", "MResearch" },
                    { "data", "X" },
                    { "action", "M03" }
                },
                new RouteValueDictionary
                {
                    { "controller", "MResearch" },
                    { "data", "X" },
                },
                new MResearchRouteHandler()));
        }
    }
}