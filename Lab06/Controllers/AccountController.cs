using System.Threading.Tasks;
using System.Web;
using System.Web.Mvc;
using Lab06.DTO;
using Lab06.Managers;
using Lab06.Models;
using Microsoft.AspNet.Identity;
using Microsoft.AspNet.Identity.Owin;
using Microsoft.Owin.Security;

namespace Lab06.Controllers
{
    public class AccountController : Controller
    {
        public AccountController()
        {
            // UserManager = ;
            // AuthenticationManager = HttpContext.GetOwinContext().Authentication;
        }

        public AppUserManager UserManager => HttpContext.GetOwinContext().GetUserManager<AppUserManager>();
        public IAuthenticationManager AuthenticationManager => HttpContext.GetOwinContext().Authentication;
        
        public ActionResult Register()
        {
            return View();
        }
 
        [HttpPost]
        public async Task<ActionResult> Register(RegisterDto dto)
        {
            if (ModelState.IsValid == false)
            {
                return View(dto);
            }

            var user = new User { UserName = dto.Email, Email = dto.Email };
            var result = await UserManager.CreateAsync(user, dto.Password);
            if (result.Succeeded == false)
            {            
                foreach (var error in result.Errors)
                {
                    ModelState.AddModelError("", error);
                }
                return View(dto);
            }
            
            return RedirectToAction("Login", "Account");
        }
        
        public ActionResult Login()
        {
            return View();
        }
 
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<ActionResult> Login(LoginDto dto)
        {
            if (ModelState.IsValid == false)
            {
                return View(dto);
            }

            
            var user = await UserManager.FindAsync(dto.Email, dto.Password);
            if (user == null)
            {
                ModelState.AddModelError("", "Неверный логин или пароль.");
                return View(dto);
            }
                
            var claim = await UserManager.CreateIdentityAsync(user, 
                DefaultAuthenticationTypes.ApplicationCookie);
            AuthenticationManager.SignOut();
            AuthenticationManager.SignIn(new AuthenticationProperties { IsPersistent = true }, claim);
            
            return RedirectToAction("Index", "Home");
        }
        
        public ActionResult Logout()
        {
            AuthenticationManager.SignOut();
            
            return RedirectToAction("Index", "Home");
        }
        
        [HttpPost]
        public ActionResult GoogleLogin(string returnUrl)
        {
            var properties = new AuthenticationProperties
            {
                RedirectUri = Url.Action("GoogleLoginCallback",
                    new { returnUrl = returnUrl })
            };

            HttpContext.GetOwinContext().Authentication.Challenge(properties, "Google");
            return new HttpUnauthorizedResult();
        }
        
        [Route("signin-google")]
        public async Task<ActionResult> GoogleLoginCallback()
        {
            var loginInfo = await AuthenticationManager.GetExternalLoginInfoAsync();
            var user = await UserManager.FindAsync(loginInfo.Login);

            if (user == null)
            {
                user = new User
                {
                    Email = loginInfo.Email,
                    UserName = loginInfo.DefaultUserName,
                };

                var result = await UserManager.CreateAsync(user);
                if (!result.Succeeded)
                {
                    return View("Error", result.Errors);
                }
                
                result = await UserManager.AddLoginAsync(user.Id, loginInfo.Login);
                if (!result.Succeeded)
                {
                    return View("Error", result.Errors);
                }
            }

            var claim = await UserManager.CreateIdentityAsync(user,
                DefaultAuthenticationTypes.ApplicationCookie);
            // claim.AddClaims(loginInfo.ExternalIdentity.Claims);
            AuthenticationManager.SignOut();
            AuthenticationManager.SignIn(new AuthenticationProperties
            {
                IsPersistent = false
            }, claim);

            return RedirectToAction("Index", "Home");
        }
    }
}