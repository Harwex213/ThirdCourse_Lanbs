using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Threading.Tasks;
using Lab02.Domain.Interfaces;
using Lab02.Domain.Models;
using Lab02.Infrastructure.EF.Entities;

namespace Lab02.Infrastructure.EF.Repositories
{
    public class EfBaseRepository<TEntity, TModel> : IBaseRepository<TModel> 
        where TModel : BaseModel 
        where TEntity : BaseEntity
    {
        protected readonly DbSet<TEntity> DbSet;
        protected readonly AppDbContext DbContext;
        protected readonly IMapper<TEntity, TModel> Mapper;
        
        public EfBaseRepository(AppDbContext dbContext, IMapper<TEntity, TModel> mapper)
        {
            DbContext = dbContext;
            Mapper = mapper;
            DbSet = dbContext.Set<TEntity>();
        }
        
        public ICollection<TModel> GetAll()
        {
            return Mapper.MapAll(DbSet.ToList());
        }

        public TModel Find(int id)
        {
            return Mapper.Map(DbSet.Find(id));
        }

        public void Create(TModel model)
        {
            DbSet.Add(Mapper.Map(model));
        }

        public void Update(TModel model)
        {
            var entityToUpdate = DbSet.FirstOrDefault(entity => entity.Id == model.Id);

            entityToUpdate?.Clone(Mapper.Map(model));
        }

        public void Delete(TModel model)
        {
            var entityToDelete = DbSet.Find(model.Id);
            if (entityToDelete != null)
            {
                DbSet.Remove(entityToDelete);
            }
        }

        public async Task SaveAsync()
        {
            await DbContext.SaveChangesAsync();
        }
    }
}