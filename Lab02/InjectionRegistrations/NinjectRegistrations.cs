using Lab02.Domain.Interfaces;
using Lab02.Infrastructure.EF;
using Lab02.Infrastructure.EF.Repositories;
using Lab02.Infrastructure.JSON;
using Lab02.Infrastructure.JSON.Repositories;
using Ninject.Modules;

namespace Lab02.InjectionRegistrations
{
    public class NinjectRegistrations : NinjectModule
    {
        public override void Load()
        {
            BindDataSourceJson();
            // BindDataSourceDb();
        }

        private void BindDataSourceJson()
        {
            Bind<JsonStorage>().ToSelf().InSingletonScope();

            Bind<IOperatorRepository>().To<JsonOperatorRepository>();
            Bind<IRecordRepository>().To<JsonRecordRepository>();
        }

        private void BindDataSourceDb()
        {
            Bind<AppDbContext>().ToSelf().InSingletonScope();

            Bind<IOperatorRepository>().To<EfOperatorRepository>();
            Bind<IRecordRepository>().To<EfRecordRepository>();
        }
    }
}