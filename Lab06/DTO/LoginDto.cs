using System.ComponentModel.DataAnnotations;

namespace Lab06.DTO
{
    public class LoginDto
    {
        [Required]
        public string Email { get; set; }
        [Required]
        [DataType(DataType.Password)]
        public string Password { get; set; }
    }
}