using System.Collections.Generic;
using System.Threading.Tasks;
using Lab02.Domain.Models;

namespace Lab02.Domain.Interfaces
{
    public interface IStorage
    {
        Task SaveChangesAsync();
        ICollection<T> GetCollection<T>() where T : BaseModel;
    }
}