using System;
using System.Collections.Generic;

namespace Lab02.Domain.Models
{
    public class Record : BaseModel
    {
        public string OwnerName { get; set; }
        public Operator Operator { get; set; }
        public string Number { get; set; }
        
        public override string ToString()
        {
            return "+375" + Operator.Code + Number;
        }
    }
    
    public class RecordComparer : IComparer<Record>
    {
        public int Compare(Record x, Record y)
        {
            if (ReferenceEquals(x, y))
            {
                return 0;
            }

            if (ReferenceEquals(null, y))
            {
                return 1;
            }

            if (ReferenceEquals(null, x))
            {
                return -1;
            }

            var result = string.Compare(x.OwnerName, y.OwnerName, StringComparison.Ordinal);
            return result == 0 ? 1 : result;
        }
    }
}