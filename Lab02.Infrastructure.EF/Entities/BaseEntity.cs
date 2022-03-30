using System.ComponentModel.DataAnnotations;

namespace Lab02.Infrastructure.EF.Entities
{
    public abstract class BaseEntity
    {
        [Key]
        public int Id { get; set; }

        public abstract void Clone(BaseEntity from);
    }
}