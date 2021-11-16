const table = require("../dto/tables").teachers;
const crudController = require("./crudController");
const router = require("../router/router")();

const getSubjects = async (request, response) => {
    await crudController.getModels({ table }, request, response);
}

const postSubject = async (request, response) => {
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
    await crudController.postModel({ table, id, values }, request, response);
};

const putSubject = async (request, response) => {
    const body = request.body;
    const id = {
        name: "teacher",
        value: body.teacher
    };
    const values = {
        teacher_name: body.teacher_name,
        pulpit: body.pulpit
    };
    await crudController.putModel({ table, id, values }, request, response);
};

const deleteSubject = async (request, response) => {
    const id = {
        name: "teacher",
        value: request.params.teacherId.toString()
    };
    await crudController.deleteModel({ table, id }, request, response);
};

router.get("/", getSubjects);
router.post("/", postSubject);
router.put("/", putSubject)
router.delete("/:teacherId", deleteSubject);

module.exports = router;