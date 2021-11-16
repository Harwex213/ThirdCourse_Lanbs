const table = require("../dto/tables").subjects;
const crudController = require("./crudController");
const router = require("../router/router")();

const getSubjects = async (request, response) => {
    await crudController.getModels({ table }, request, response);
}

const postSubject = async (request, response) => {
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
    await crudController.postModel({ table, id, values }, request, response);
};

const putSubject = async (request, response) => {
    const body = request.body;
    const id = {
        name: "subject",
        value: body.subject
    };
    const values = {
        subject_name: body.subject_name,
        pulpit: body.pulpit
    };
    await crudController.putModel({ table, id, values }, request, response);
};

const deleteSubject = async (request, response) => {
    const id = {
        name: "subject",
        value: request.params.subjectId.toString()
    };
    await crudController.deleteModel({ table, id }, request, response);
};

router.get("/", getSubjects);
router.post("/", postSubject);
router.put("/", putSubject)
router.delete("/:subjectId", deleteSubject);

module.exports = router;