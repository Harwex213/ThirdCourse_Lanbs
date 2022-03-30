using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Lab02.Domain.Interfaces;
using Lab02.Domain.Models;

namespace Lab02.Infrastructure.JSON.Repositories
{
    public class JsonBaseRepository<T> : IBaseRepository<T> where T : BaseModel
    {
        protected readonly ICollection<T> Collection;
        protected readonly JsonStorage Storage;

        public JsonBaseRepository(JsonStorage storage)
        {
            Storage = storage;
            Collection = storage.GetCollection<T>();
        }
        
        public ICollection<T> GetAll()
        {
            return Collection;
        }

        public T Find(int id)
        {
            return Collection.FirstOrDefault(item => item.Id == id);
        }

        public void Create(T entity)
        {
            entity.Id = Collection.Count == 0 ? 0 : Collection.Max(item => item.Id) + 1;
            Collection.Add(entity);
        }

        public void Update(T entity)
        {
            var entityToUpdate = Collection.FirstOrDefault(item => item.Id == entity.Id);
            if (entityToUpdate != null)
            {
                Collection.Remove(entityToUpdate);
                Collection.Add(entity);
            }
        }

        public void Delete(T entity)
        {
            var entityToDelete = Collection.FirstOrDefault(item => item.Id == entity.Id);
            if (entityToDelete != null)
            {
                Collection.Remove(entityToDelete);
            }
        }

        public async Task SaveAsync()
        {
            await Storage.SaveChangesAsync();
        }
    }
}