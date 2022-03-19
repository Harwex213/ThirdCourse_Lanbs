using System.ComponentModel.DataAnnotations;

namespace Lab02.DTOs.Dict
{
    public class UpdateDto
    {
        [Required]
        public int Id { get; set; }
        [Required]
        [RegularExpression(@"^[A-Z]{1}[a-z]{1,24}$", ErrorMessage = "Пожалуйста, введите валидное имя")]
        public string Name { get; set; }
        [Required]
        public int Code { get; set; }
        [Required]
        [RegularExpression(@"^[0-9]{3}[0-9]{3}[0-9]{2}$", ErrorMessage = "Пожалуйста, введите валидный номер")]
        public string Number { get; set; }
    }
}