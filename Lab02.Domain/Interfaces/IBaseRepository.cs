using System.Collections;
using System.Collections.Generic;
using System.Threading.Tasks;
using Lab02.Domain.Models;

namespace Lab02.Domain.Interfaces
{
    public interface IBaseRepository<T> where T : BaseModel
    {
        ICollection<T> GetAll();
        T Find(int id);
        void Create(T entity);
        void Update(T entity);
        void Delete(T entity);
        Task SaveAsync();
    }
}