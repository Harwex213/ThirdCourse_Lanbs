using System.Collections.Generic;
using System.Threading.Tasks;
using System.Web.Mvc;
using Lab02.Domain.Interfaces;
using Lab02.Domain.Models;
using Lab02.DTOs.Dict;

namespace Lab02.Controllers
{
    public class DictController : Controller
    {
        private IOperatorRepository _operatorRepository;
        private IRecordRepository _recordRepository;
        
        public DictController(IOperatorRepository operatorRepository, IRecordRepository recordRepository)
        {
            _operatorRepository = operatorRepository;
            _recordRepository = recordRepository;
        }
        
        public ActionResult Index()
        {
            return View(_recordRepository.GetAll());
        }
        
        public ActionResult Add()
        {
            ViewBag.Operators = _operatorRepository.GetAll();
            return View(new AddDto());
        }
        
        [HttpPost]
        public async Task<ActionResult> Add(AddDto addDto)
        {
            if (!ModelState.IsValid)
            {
                return RedirectToAction(nameof(Add));
            }
            _recordRepository.Create(new Record
            {
                OwnerName = addDto.Name,
                Operator = new Operator { Code = addDto.Code },
                Number = addDto.Number
            });
            await _recordRepository.SaveAsync();

            return RedirectToAction(nameof(Index));
        }
        
        public ActionResult Update(int id, RecordDto recordDto)
        {
            ViewBag.Operators = _operatorRepository.GetAll();
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
            _recordRepository.Update(new Record
            {
                Id = updateDto.Id,
                OwnerName = updateDto.Name,
                Operator = new Operator { Code = updateDto.Code },
                Number = updateDto.Number
            });
            await _recordRepository.SaveAsync();

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
            _recordRepository.Delete(new Record
            {
                Id = deleteDto.Id
            });
            await _recordRepository.SaveAsync();

            return RedirectToAction(nameof(Index));
        }
    }
}