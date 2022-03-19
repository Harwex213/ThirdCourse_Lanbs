using System.Collections;
using System.Collections.Generic;

namespace Lab02.Models
{
    static class Storage
    {
        static Storage()
        {
            Records = new SortedSet<Record>(new RecordComparer());
            Operators = new List<Operator>
            {
                new Operator
                {
                    Code = 29
                },
                new Operator
                {
                    Code = 44
                },
                new Operator
                {
                    Code = 33
                },
            };
        }
        
        public static ICollection<Record> Records { get; set; }
        public static ICollection<Operator> Operators { get; set; }
    }
}