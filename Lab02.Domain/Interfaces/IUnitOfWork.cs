using System.Threading.Tasks;
using Lab02.Domain.Models;

namespace Lab02.Domain.Interfaces
{
    public interface IUnitOfWork
    {
        Task<int> CommitAsync();
        IRepository<T> Repository<T>() where T : BaseModel;
    }
}