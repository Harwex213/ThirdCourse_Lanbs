using System.Collections.Generic;
using System.Linq;
using Lab08.Models;
using Microsoft.EntityFrameworkCore;

namespace Lab08.DataAccess
{
    public class UserRepository : IUserRepository
    {
        public UserRepository(AppDbContext dbContext)
        {
            DbContext = dbContext;
            Users = dbContext.Users;
        }

        public AppDbContext DbContext { get; set; }
        public DbSet<User> Users { get; set; }
        
        public IEnumerable<User> GetAll()
        {
            return Users.ToList();
        }

        public User GetUser(int id)
        {
            return Users.Find(id);
        }

        public void AddUser(User user)
        {
            Users.Add(user);
            DbContext.SaveChanges();
        }

        public void UpdateUser(User user)
        {
            Users.Update(user);
            DbContext.SaveChanges();
        }

        public bool DeleteUser(int id)
        {
            var userToDelete = Users.Find(id);
            if (userToDelete == null)
            {
                return false;
            }

            Users.Remove(userToDelete);
            DbContext.SaveChanges();
            return true;
        }
    }
}