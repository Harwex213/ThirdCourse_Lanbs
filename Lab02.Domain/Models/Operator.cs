namespace Lab02.Domain.Models
{
    public class Operator : BaseModel
    {
        public int Code { get; set; }

        public override string ToString()
        {
            return Code.ToString();
        }
    }
}