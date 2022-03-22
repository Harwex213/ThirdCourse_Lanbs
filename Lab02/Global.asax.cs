using System.Web.Mvc;
using System.Web.Optimization;
using System.Web.Routing;
using Lab02.InjectionRegistrations;
using Ninject;
using Ninject.Web.Mvc;

namespace Lab02
{
    public class MvcApplication : System.Web.HttpApplication
    {
        protected void Application_Start()
        {
            AreaRegistration.RegisterAllAreas();
            FilterConfig.RegisterGlobalFilters(GlobalFilters.Filters);
            RouteConfig.RegisterRoutes(RouteTable.Routes);
            BundleConfig.RegisterBundles(BundleTable.Bundles);
            
            var registrations = new NinjectRegistrations();
            var kernel = new StandardKernel(registrations);
            kernel.Unbind<ModelValidatorProvider>();
            DependencyResolver.SetResolver(new NinjectDependencyResolver(kernel));
        }
    }
}