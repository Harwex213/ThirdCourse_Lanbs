using Lab02.Domain.Models;

namespace Lab02.Domain.Interfaces
{
    public interface IOperatorRepository : IBaseRepository<Operator>
    {
        Operator GetByCode(int code);
    }
}