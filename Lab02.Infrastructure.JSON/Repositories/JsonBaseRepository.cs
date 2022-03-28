using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Lab02.Domain.Interfaces;
using Lab02.Domain.Models;

namespace Lab02.Infrastructure.JSON.Repositories
{
    public class JsonBaseRepository<T> : IBaseRepository<T> where T : BaseModel
    {
        private readonly ICollection<T> _collection;
        private readonly JsonStorage _storage;

        public JsonBaseRepository(JsonStorage storage)
        {
            _storage = storage;
            _collection = storage.GetCollection<T>();
        }
        
        public ICollection<T> GetAll()
        {
            return _collection;
        }

        public T Find(int id)
        {
            return _collection.FirstOrDefault(item => item.Id == id);
        }

        public void Create(T entity)
        {
            entity.Id = _collection.Count == 0 ? 0 : _collection.Max(item => item.Id) + 1;
            _collection.Add(entity);
        }

        public void Update(T entity)
        {
            var entityToUpdate = _collection.FirstOrDefault(item => item.Id == entity.Id);
            if (entityToUpdate != null)
            {
                _collection.Remove(entityToUpdate);
                _collection.Add(entity);
            }
        }

        public void Delete(T entity)
        {
            var entityToDelete = _collection.FirstOrDefault(item => item.Id == entity.Id);
            if (entityToDelete != null)
            {
                _collection.Remove(entityToDelete);
            }
        }

        public async Task SaveAsync()
        {
            await _storage.SaveChangesAsync();
        }
    }
}