using System.Collections.Generic;

namespace Lab08.Models
{
    public interface IUserRepository
    {
        IEnumerable<User> GetAll();
        User GetUser(int id);
        void AddUser(User user);
        void UpdateUser(User user);
        bool DeleteUser(int id);
    }
}