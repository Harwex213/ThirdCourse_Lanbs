using System.Web.Mvc;

namespace Lab05.B.Controllers
{
    [RoutePrefix("it")]
    public class MResearchController : Controller
    {
        [HttpGet]
        [Route("{n:int}/{str}")]
        public string M01(int n, string str)
        {
            return $"GET:MO1:/{n}/{str}";
        }
        
        [AcceptVerbs("GET", "POST")]
        [Route("{b:bool}/{letters:alpha}")] 
        public string M02(bool b, string letters)
        {
            return $"{Request.HttpMethod}:M02:/{b}/{letters}";
        }
        
        [AcceptVerbs("GET", "DELETE")]
        [Route("{f:float}/{letters:length(2, 5)}")]
        public string M03(float f, string letters)
        {
            return $"{Request.HttpMethod}:M03:/{f}/{letters}";
        }

        [AcceptVerbs("get", "put")]
        [Route("{letters:alpha:length(3,4)}/{b:int:range(100,200)}")]
        public string M04(string letters, int b)
        {
            return $"Put:M04:/{letters}/{b}";
        }
        
        [HttpPost]
        [Route(@"{mail:regex(^[\w-\.]+@([\w-]+\.)+[\w-]+$)}")]
        public string M04(string mail)
        {
            return $"Post:/{mail}";
        }
    }
}