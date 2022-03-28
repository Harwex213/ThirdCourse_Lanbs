using Lab02.Domain.Interfaces;
using Lab02.Domain.Models;
using Lab02.Infrastructure.EF.Entities;

namespace Lab02.Infrastructure.EF.Repositories
{
    public class EfOperatorRepository : EfBaseRepository<OperatorEntity, Operator>, IOperatorRepository
    {
        public EfOperatorRepository(AppDbContext dbContext) : base(dbContext, new OperatorEntityMapper())
        {
        }
    }
}