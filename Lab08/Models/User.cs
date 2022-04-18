using System.ComponentModel.DataAnnotations;

namespace Lab08.Models
{
    public class User
    {        
        [Required]
        public int Id { get; set; }

        public string FirstName { get; set; }

        public string LastName { get; set; }
        
        [Required]
        [EmailAddress]
        public string Email { get; set; }
        
        [Required]
        public string Password { get; set; }
        
        [Required]
        [RegularExpression("^(active|passive)$")]
        public string Status { get; set; }
        
        [Required]
        [RegularExpression("^(admin|customer|HR)$")]
        public string Role { get; set; }
    }
}