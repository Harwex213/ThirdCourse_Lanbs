using System.Threading.Tasks;
using System.Web.Mvc;
using Lab02.Domain.Interfaces;
using Lab02.Domain.Models;
using Lab02.DTOs.Dict;

namespace Lab02.Controllers
{
    public class DictController : Controller
    {
        private IUnitOfWork _unitOfWork;
        
        public DictController(IUnitOfWork unitOfWork)
        {
            _unitOfWork = unitOfWork;
        }
        
        public ActionResult Index()
        {
            return View(_unitOfWork.Repository<Record>().GetAll());
        }
        
        public ActionResult Add()
        {
            ViewBag.Operators = _unitOfWork.Repository<Operator>().GetAll();
            return View(new AddDto());
        }
        
        [HttpPost]
        public async Task<ActionResult> Add(AddDto addDto)
        {
            if (!ModelState.IsValid)
            {
                return RedirectToAction(nameof(Add));
            }
            _unitOfWork.Repository<Record>().Add(new Record
            {
                OwnerName = addDto.Name,
                Operator = new Operator { Code = addDto.Code },
                Number = addDto.Number
            });
            await _unitOfWork.CommitAsync();

            return RedirectToAction(nameof(Index));
        }
        
        public ActionResult Update(int id, RecordDto recordDto)
        {
            ViewBag.Operators = _unitOfWork.Repository<Operator>().GetAll();
            return View(new UpdateDto
            {
                Id = id,
                Name = recordDto.Name,
                Code = recordDto.Code,
                Number = recordDto.Number
            });
        }
        
        [HttpPost]
        public async Task<ActionResult> Update(UpdateDto updateDto)
        {
            if (!ModelState.IsValid)
            {
                return RedirectToAction(nameof(Update));
            }
            _unitOfWork.Repository<Record>().Update(new Record
            {
                Id = updateDto.Id,
                OwnerName = updateDto.Name,
                Operator = new Operator { Code = updateDto.Code },
                Number = updateDto.Number
            });
            await _unitOfWork.CommitAsync();

            return RedirectToAction(nameof(Index));
        }

        public ActionResult Delete(int id)
        {
            return View(new DeleteDto
            {
                Id = id
            });
        }
        
        [HttpPost]
        public async Task<ActionResult> Delete(DeleteDto deleteDto)
        {
            if (!ModelState.IsValid)
            {
                return RedirectToAction(nameof(Update));
            }
            _unitOfWork.Repository<Record>().Delete(new Record
            {
                Id = deleteDto.Id
            });
            await _unitOfWork.CommitAsync();
            
            return RedirectToAction(nameof(Index));
        }
    }
}