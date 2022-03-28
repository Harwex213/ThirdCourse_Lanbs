using Lab02.Domain.Interfaces;
using Lab02.Domain.Models;
using Lab02.Infrastructure.EF.Entities;

namespace Lab02.Infrastructure.EF.Repositories
{
    public class EfRecordRepository : EfBaseRepository<RecordEntity, Record>, IRecordRepository
    {
        public EfRecordRepository(AppDbContext dbContext) : base(dbContext, new RecordEntityMapper())
        {
        }
    }
}