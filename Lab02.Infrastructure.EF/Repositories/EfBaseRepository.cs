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
        where TEntity : class
    {
        private readonly DbSet<TEntity> _dbSet;
        private readonly AppDbContext _dbContext;
        private readonly IMapper<TEntity, TModel> _mapper;
        
        public EfBaseRepository(AppDbContext dbContext, IMapper<TEntity, TModel> mapper)
        {
            _dbContext = dbContext;
            _mapper = mapper;
            _dbSet = dbContext.Set<TEntity>();
        }
        
        public ICollection<TModel> GetAll()
        {
            return _mapper.MapAll(_dbSet.ToList());
        }

        public TModel Find(int id)
        {
            return _mapper.Map(_dbSet.Find(id));
        }

        public void Create(TModel model)
        {
            _dbSet.Add(_mapper.Map(model));
        }

        public void Update(TModel model)
        {
            var entity = _mapper.Map(model);
            var entityToUpdate = _dbSet.Find(model.Id);
            if (entityToUpdate != null)
            {
                entityToUpdate = entity;
            }
        }

        public void Delete(TModel model)
        {
            var entityToDelete = _dbSet.Find(model.Id);
            if (entityToDelete != null)
            {
                _dbSet.Remove(entityToDelete);
            }
        }

        public async Task SaveAsync()
        {
            await _dbContext.SaveChangesAsync();
        }
    }
}