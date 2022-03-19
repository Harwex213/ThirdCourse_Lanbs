namespace Lab02.Models
{
    public class Operator
    {
        public int Code { get; set; }

        public override string ToString()
        {
            return Code.ToString();
        }
    }
}