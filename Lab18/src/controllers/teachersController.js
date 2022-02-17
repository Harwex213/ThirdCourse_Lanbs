const express = require("express");
const model = require("../constants/database").Teacher;
const crudController = require("./crudController");

const getSubjects = (request, response, next) => {
    next({ model });
}

const postSubject = async (request, response, next) => {
    const body = request.body;
    const id = {
        TEACHER: body.teacher
    };
    const values = {
        TEACHER: body.teacher,
        TEACHER_NAME: body.teacher_name,
        GENDER: body.gender,
        PULPIT: body.pulpit
    };
    next({ model, id, values });
};

const putSubject = async (request, response, next) => {
    const body = request.body;
    const id = {
        TEACHER: body.teacher
    };
    const values = {
        TEACHER_NAME: body.teacher_name,
        GENDER: body.gender,
        PULPIT: body.pulpit
    };
    next({ model, id, values });
};

const deleteSubject = async (request, response, next) => {
    const id = {
        TEACHER: request.params.teacherId.toString()
    };
    next({ model, id });
};

const router = express.Router();

router.get("/", getSubjects, crudController.getModels);
router.post("/", postSubject, crudController.postModel);
router.put("/", putSubject, crudController.putModel)
router.delete("/:teacherId", deleteSubject, crudController.deleteModel);

module.exports = router;