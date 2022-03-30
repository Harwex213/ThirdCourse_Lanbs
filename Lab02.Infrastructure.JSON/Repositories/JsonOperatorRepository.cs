using System.Linq;
using Lab02.Domain.Interfaces;
using Lab02.Domain.Models;

namespace Lab02.Infrastructure.JSON.Repositories
{
    public class JsonOperatorRepository : JsonBaseRepository<Operator>, IOperatorRepository
    {
        public JsonOperatorRepository(JsonStorage storage) : base(storage)
        {
        }

        public Operator GetByCode(int code)
        {
            return Storage.Operators.FirstOrDefault(@operator => @operator.Code == code);
        }
    }
}