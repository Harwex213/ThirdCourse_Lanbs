using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using Lab06.Managers;
using Lab06.Models;
using Microsoft.AspNet.Identity;
using Microsoft.AspNet.Identity.EntityFramework;

namespace Lab06.Context
{
    public class AppDbContext : IdentityDbContext<User>
    {
        public AppDbContext() : base("DefaultConnection", throwIfV1Schema: false)
        {
        }

        public static AppDbContext Create()
        {
            return new AppDbContext();
        }
    }
    
    public class AppDbInitializer : DropCreateDatabaseAlways<AppDbContext>
    {
        protected override void Seed(AppDbContext context)
        {
            var userManager = new AppUserManager(new UserStore<User>(context));
            var roleManager = new RoleManager<IdentityRole>(new RoleStore<IdentityRole>(context));

            var roles = new List<IdentityRole>
            {
                new IdentityRole { Name = "Administrator" },
                new IdentityRole { Name = "Employer" },
                new IdentityRole { Name = "Guest" },
            };
            foreach (var role in roles)
            {
                roleManager.Create(role);
            }

            var users = new List<(User user, string password, string[] roles)>
            {
                (
                    new User { Email = "admin@belstu.by", UserName = "admin@belstu.by" },
                    "111111",
                    new [] { "Administrator" }
                ),
                (
                    new User { Email = "emp@belstu.by", UserName = "emp@belstu.by" },
                    "111111",
                    new [] { "Employer" }
                ),
                (
                    new User { Email = "guest@belstu.by", UserName = "guest@belstu.by" },
                    "111111",
                    new [] { "Guest" }
                ),
                (
                    new User { Email = "super@belstu.by", UserName = "super@belstu.by" },
                    "111111",
                    new [] { "Administrator", "Employer", "Guest" }
                ),
            };
            foreach (var (user, password, userRoles) in users)
            {
                if (!userManager.Create(user, password).Succeeded)
                {
                    continue;
                }

                var receivedRoles = roles.Select(role => role.Name).Intersect(userRoles);
                foreach (var role in receivedRoles)
                {
                    userManager.AddToRole(user.Id, role);
                }
            }
            base.Seed(context);
        }
    }
}