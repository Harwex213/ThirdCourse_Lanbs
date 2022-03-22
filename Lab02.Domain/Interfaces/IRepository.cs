using System.Collections.Generic;
using Lab02.Domain.Models;

namespace Lab02.Domain.Interfaces
{
    public interface IRepository<T> where T : BaseModel
    {
        void Add(T entity);
        void Delete(T entity);
        void Update(T entity);
        T Find(params object[] keys);
        IList<T> GetAll();
    }
}