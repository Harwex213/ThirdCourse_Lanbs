using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using Lab02.Domain.Models;
using Lab02.Infrastructure.EF.Repositories;

namespace Lab02.Infrastructure.EF.Entities
{
    [Table("Record")]
    public class RecordEntity : BaseEntity
    {
        [Required]
        public int OperatorCode { get; set; }
        
        [Required]
        public string OwnerName { get; set; }
        
        [Required]
        public string Number { get; set; }
        
        [ForeignKey("OperatorCode")]
        public virtual OperatorEntity Operator { get; set; }

        public override void Clone(BaseEntity @from)
        {
            if (@from is RecordEntity recordFrom)
            {
                Number = recordFrom.Number;
                OwnerName = recordFrom.OwnerName;
                OperatorCode = recordFrom.OperatorCode;
            }
        }
    }
    
    public class RecordEntityMapper : IMapper<RecordEntity, Record>
    {
        public Record Map(RecordEntity entity)
        {
            return new Record
            {
                Id = entity.Id,
                Operator = new Operator
                {
                    Id = entity.Operator.Id,
                    Code = entity.Operator.Code
                },
                OwnerName = entity.OwnerName,
                Number = entity.Number
            };
        }

        public RecordEntity Map(Record model)
        {
            return new RecordEntity
            {
                Id = model.Id,
                OperatorCode = model.Operator.Id,
                OwnerName = model.OwnerName,
                Number = model.Number
            };
        }

        public ICollection<Record> MapAll(ICollection<RecordEntity> entities)
        {
            return entities.Select(Map).ToList();
        }

        public ICollection<RecordEntity> MapAll(ICollection<Record> models)
        {
            return models.Select(Map).ToList();
        }
    }
}