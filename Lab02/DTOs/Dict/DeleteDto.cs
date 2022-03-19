using System.ComponentModel.DataAnnotations;

namespace Lab02.DTOs.Dict
{
    public class DeleteDto
    {
        [Required]
        public int Id { get; set; }
    }
}