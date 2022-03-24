using Lab02.Domain.Interfaces;
using Lab02.Domain.Models;

namespace Lab02.Infrastructure.JSON.Repositories
{
    public class JsonOperatorRepository : JsonBaseRepository<Operator>, IOperatorRepository
    {
        public JsonOperatorRepository(JsonStorage storage) : base(storage)
        {
            
        }
    }
}