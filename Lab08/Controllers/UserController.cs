using System.Collections;
using System.Collections.Generic;
using Lab08.Models;
using Microsoft.AspNetCore.Mvc;

namespace Lab08.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class UserController : ControllerBase
    {
        public UserController(IUserRepository userRepository)
        {
            UserRepository = userRepository;
        }

        public IUserRepository UserRepository { get; set; }

        [HttpGet]
        public ActionResult<IEnumerable<User>> GetUsers()
        {
            return Ok(UserRepository.GetAll());
        }

        [HttpGet("{id:int}")]
        public ActionResult<User> GetUser(int id)
        {
            var user = UserRepository.GetUser(id);

            return user != null ? Ok(user) : NotFound();
        }
        
        [HttpPost]
        public ActionResult<User> PostUser([FromBody] User user)
        {
            UserRepository.AddUser(user);
            return CreatedAtAction(nameof(GetUser), new { id = user.Id }, user);
        }
        
        [HttpPut]
        public ActionResult<User> PutUser([FromBody] User user)
        {
            if (UserRepository.GetUser(user.Id) == null)
            {
                return BadRequest();
            };

            UserRepository.UpdateUser(user);
            return Ok();

        }
        
        [HttpDelete("{id:int}")]
        public ActionResult<User> DeleteUser(int id)
        {
            return UserRepository.DeleteUser(id) ? Ok() : NotFound();
        }
    }
}