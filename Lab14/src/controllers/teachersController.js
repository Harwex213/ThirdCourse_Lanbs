const express = require("express");
const table = require("../services/tablesDto").teacherService;
const crudController = require("./crudController");

const getSubjects = (request, response, next) => {
    next({ table });
}

const postSubject = async (request, response, next) => {
    const body = request.body;
    const id = {
        name: "teacher",
        value: body.teacher
    };
    const values = {
        teacher: body.teacher,
        teacher_name: body.teacher_name,
        pulpit: body.pulpit
    };
    next({ table, id, values });
};

const putSubject = async (request, response, next) => {
    const body = request.body;
    const id = {
        name: "teacher",
        value: body.teacher
    };
    const values = {
        teacher_name: body.teacher_name,
        pulpit: body.pulpit
    };
    next({ table, id, values });
};

const deleteSubject = async (request, response, next) => {
    const id = {
        name: "teacher",
        value: request.params.teacherId.toString()
    };
    next({ table, id });
};

const router = express.Router();

router.get("/", getSubjects, crudController.getModels);
router.post("/", postSubject, crudController.postModel);
router.put("/", putSubject, crudController.putModel)
router.delete("/:teacherId", deleteSubject, crudController.deleteModel);

module.exports = router;