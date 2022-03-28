using System.Collections;
using System.Collections.Generic;
using Lab02.Domain.Models;

namespace Lab02.Infrastructure.EF.Repositories
{
    public interface IMapper<TEntity, TModel>
        where TEntity : class
        where TModel : BaseModel
    {
        TModel Map(TEntity entity);
        TEntity Map(TModel model);

        ICollection<TModel> MapAll(ICollection<TEntity> entities);
        ICollection<TEntity> MapAll(ICollection<TModel> models);
    }
}