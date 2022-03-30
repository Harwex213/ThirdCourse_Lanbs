using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using Lab02.Domain.Models;
using Lab02.Infrastructure.EF.Repositories;

namespace Lab02.Infrastructure.EF.Entities
{
    [Table("Operator")]
    public class OperatorEntity : BaseEntity
    {
        public OperatorEntity()
        {
            Records = new List<RecordEntity>();
        }

        [Required]
        public int Code { get; set; }

        public virtual IEnumerable<RecordEntity> Records { get; set; }
        
        public override void Clone(BaseEntity @from)
        {
            if (@from is OperatorEntity operatorFrom)
            {
                Code = operatorFrom.Code;
            }
        }
    }
    
    public class OperatorEntityMapper : IMapper<OperatorEntity, Operator>
    {
        public Operator Map(OperatorEntity entity)
        {
            return new Operator
            {
                Id = entity.Id,
                Code = entity.Code
            };
        }

        public OperatorEntity Map(Operator model)
        {
            return new OperatorEntity
            {
                Id = model.Id,
                Code = model.Code
            };
        }

        public ICollection<Operator> MapAll(ICollection<OperatorEntity> entities)
        {
            return entities.Select(Map).ToList();
        }

        public ICollection<OperatorEntity> MapAll(ICollection<Operator> models)
        {
            return models.Select(Map).ToList();
        }
    }
}