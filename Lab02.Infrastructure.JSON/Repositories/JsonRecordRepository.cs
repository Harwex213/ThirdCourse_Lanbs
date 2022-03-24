using Lab02.Domain.Interfaces;
using Lab02.Domain.Models;

namespace Lab02.Infrastructure.JSON.Repositories
{
    public class JsonRecordRepository : JsonBaseRepository<Record>,  IRecordRepository
    {
        public JsonRecordRepository(JsonStorage storage) : base(storage)
        {
            
        }
        
        
    }
}