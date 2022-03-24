using Lab02.Domain.Interfaces;
using Lab02.Infrastructure.JSON;
using Lab02.Infrastructure.JSON.Repositories;
using Ninject.Modules;

namespace Lab02.InjectionRegistrations
{
    public class NinjectRegistrations : NinjectModule
    {
        public override void Load()
        {
            Bind<JsonStorage>().ToSelf().InSingletonScope();

            Bind<IOperatorRepository>().To<JsonOperatorRepository>();
            Bind<IRecordRepository>().To<JsonRecordRepository>();
        }
    }
}