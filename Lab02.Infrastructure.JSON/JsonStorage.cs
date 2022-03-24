using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using Lab02.Domain.Interfaces;
using Lab02.Domain.Models;
using Newtonsoft.Json;

namespace Lab02.Infrastructure.JSON
{
    public class JsonStorage : IStorage
    {
        private static string _storageLocation =
            @"D:\Wordplace\2_Blue\1University\third_course_2\ASP.NET\Data";
        private readonly string _recordsLocation = _storageLocation + @"\Records.json";
        private readonly string _operatorsLocation = _storageLocation + @"\Operators.json";
        
        public SortedSet<Record> Records { get; set; }
        public List<Operator> Operators { get; set; }
        
        public JsonStorage()
        {
            DeserializeObjects();
        }

        private void DeserializeObjects()
        {
            using (var reader = new StreamReader(_recordsLocation))
            {
                var recordList = JsonConvert.DeserializeObject<List<Record>>(reader.ReadToEnd()) ?? new List<Record>();
                Records = new SortedSet<Record>(recordList, new RecordComparer());;
            }
            
            using (var reader = new StreamReader(_operatorsLocation))
            {
                Operators = JsonConvert.DeserializeObject<List<Operator>>(reader.ReadToEnd()) ?? new List<Operator>();
            }
        }
        
        public async Task SaveChangesAsync()
        {
            using (var writer = new StreamWriter(_recordsLocation, false))
            {
                await writer.WriteAsync(JsonConvert.SerializeObject(Records));
            }

            using (var writer = new StreamWriter(_operatorsLocation, false))
            {
                await writer.WriteAsync(JsonConvert.SerializeObject(Operators));
            }
        }

        public ICollection<T> GetCollection<T>() where T : BaseModel
        {
            // TODO: Use reflection
            
            if (typeof(T) == typeof(Record))
            {
                return (ICollection<T>)Records;
            }
            if (typeof(T) == typeof(Operator))
            {
                return (ICollection<T>)Operators;
            }
            
            return (ICollection<T>)Records;
        }
    }
}