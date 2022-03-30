using System.Collections.Generic;
using Lab02.Domain.Models;

namespace Lab02.Domain.Interfaces
{
    public interface IRecordRepository : IBaseRepository<Record>
    {
        ICollection<Record> GetAllSorted();
    }
}