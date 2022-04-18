using Lab08.Models;
using Microsoft.EntityFrameworkCore;

namespace Lab08.DataAccess
{
    public sealed class AppDbContext : DbContext
    {
        public DbSet<User> Users { get; set; }
        
        public AppDbContext(DbContextOptions<AppDbContext> options) : base(options)
        {
            Database.EnsureCreated();
        }
    }
}