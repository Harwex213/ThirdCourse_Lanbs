using System;
using System.Collections.Generic;
using System.Linq;
using System.Web.Mvc;
using Lab02.DTOs.Dict;
using Lab02.Models;
using Newtonsoft.Json.Serialization;

namespace Lab02.Controllers
{
    public class DictController : Controller
    {
        public ActionResult Index()
        {
            return View(Storage.Records);
        }
        
        public ActionResult Add()
        {
            ViewBag.Operators = Storage.Operators;
            return View(new AddDto());
        }
        
        [HttpPost]
        public ActionResult Add(AddDto addDto)
        {
            if (!ModelState.IsValid)
            {
                return RedirectToAction(nameof(Add));
            }
            Storage.Records.Add(new Record
            {
                Id = Storage.Records.Count,
                OwnerName = addDto.Name,
                Operator = new Operator { Code = addDto.Code },
                Number = addDto.Number
            });

            return RedirectToAction(nameof(Index));
        }
        
        public ActionResult Update(int id, RecordDto recordDto)
        {
            ViewBag.Operators = Storage.Operators;
            ViewBag.Name = recordDto.Name;
            ViewBag.Code = recordDto.Code;
            ViewBag.Number = recordDto.Number;
            return View(new UpdateDto
            {
                Id = id
            });
        }
        
        [HttpPost]
        public ActionResult Update(UpdateDto updateDto)
        {
            var recordToUpdate = Storage.Records.FirstOrDefault(record => record.Id == updateDto.Id);
            if (recordToUpdate != null)
            {
                recordToUpdate.OwnerName = updateDto.Name;
                recordToUpdate.Operator = new Operator { Code = updateDto.Code };
                recordToUpdate.Number = updateDto.Number;
            }
            
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
        public ActionResult Delete(DeleteDto deleteDto)
        {
            var recordToDelete = Storage.Records.FirstOrDefault(record => record.Id == deleteDto.Id);
            if (recordToDelete != null)
            {
                Storage.Records.Remove(recordToDelete);
            }
            
            return RedirectToAction(nameof(Index));
        }
    }
}