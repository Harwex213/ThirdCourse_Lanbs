const express = require("express");
const model = require("../constants/database").Subject;
const crudController = require("./crudController");

const getSubjects = (request, response, next) => {
    next({ model });
}

const postSubject = async (request, response, next) => {
    const body = request.body;
    const id = {
        SUBJECT: body.subject
    };
    const values = {
        SUBJECT: body.subject,
        SUBJECT_NAME: body.subject_name,
        PULPIT: body.pulpit
    };
    next({ model, id, values });
};

const putSubject = async (request, response, next) => {
    const body = request.body;
    const id = {
        SUBJECT: body.subject
    };
    const values = {
        SUBJECT_NAME: body.subject_name,
        PULPIT: body.pulpit
    };
    next({ model, id, values });
};

const deleteSubject = async (request, response, next) => {
    const id = {
        SUBJECT: request.params.subjectId.toString()
    };
    next({ model, id });
};

const router = express.Router();

router.get("/", getSubjects, crudController.getModels);
router.post("/", postSubject, crudController.postModel);
router.put("/", putSubject, crudController.putModel)
router.delete("/:subjectId", deleteSubject, crudController.deleteModel);

module.exports = router;