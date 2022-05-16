using System;
using Lab06.Context;
using Lab06.Models;
using Microsoft.AspNet.Identity;
using Microsoft.AspNet.Identity.EntityFramework;
using Microsoft.AspNet.Identity.Owin;
using Microsoft.Owin;

namespace Lab06.Managers
{
    public class AppUserManager : UserManager<User>
    {
        public AppUserManager(IUserStore<User> store) : base(store)
        {
        }

        public static AppUserManager Create(IdentityFactoryOptions<AppUserManager> options, IOwinContext context) 
        {
            var manager = new AppUserManager(new UserStore<User>(context.Get<AppDbContext>()));
            
            // manager.UserLockoutEnabledByDefault = false;
            
            manager.UserValidator = new UserValidator<User>(manager)
            {
                AllowOnlyAlphanumericUserNames = false,
                RequireUniqueEmail = true
            };
            manager.PasswordValidator = new PasswordValidator
            {
                RequiredLength = 6,
                // RequireNonLetterOrDigit = true,
                // RequireDigit = true,
                // RequireLowercase = true,
                // RequireUppercase = true,
            };

            // var dataProtectionProvider = options.DataProtectionProvider;
            // if (dataProtectionProvider != null)
            // {
            //     manager.UserTokenProvider = new DataProtectorTokenProvider<User>(
            //         dataProtectionProvider.Create("HARWEX Identity"));
            // }
            return manager;
        }
    }
}