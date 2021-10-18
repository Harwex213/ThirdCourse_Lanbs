const express = require("express");
const table = require("../dto/tables").subjects;
const crudController = require("./crudController");

const getSubjects = (request, response, next) => {
    next({ table });
}

const postSubject = async (request, response, next) => {
    const body = request.body;
    const id = {
        name: "subject",
        value: body.subject
    };
    const values = {
        subject: body.subject,
        subject_name: body.subject_name,
        pulpit: body.pulpit
    };
    next({ table, id, values });
};

const putSubject = async (request, response, next) => {
    const body = request.body;
    const id = {
        name: "subject",
        value: body.subject
    };
    const values = {
        subject_name: body.subject_name,
        pulpit: body.pulpit
    };
    next({ table, id, values });
};

const deleteSubject = async (request, response, next) => {
    const id = {
        name: "subject",
        value: request.params.subjectId.toString()
    };
    next({ table, id });
};

const router = express.Router();

router.get("/", getSubjects, crudController.getModels);
router.post("/", postSubject, crudController.postModel);
router.put("/", putSubject, crudController.putModel)
router.delete("/:subjectId", deleteSubject, crudController.deleteModel);

module.exports = router;