const table = require("../dto/tables").faculties;
const crudController = require("./crudController");
const router = require("../router/router")();

const getFaculties = async (request, response) => {
    await crudController.getModels({ table }, request, response);
}

const postFaculty = async (request, response) => {
    const body = request.body;
    const id = {
        name: "faculty",
        value: body.faculty
    };
    const values = {
        faculty: body.faculty,
        faculty_name: body.faculty_name,
    };
    await crudController.postModel({ table, id, values }, request, response);
};

const putFaculty = async (request, response) => {
    const body = request.body;
    const id = {
        name: "faculty",
        value: body.faculty
    };
    const values = {
        faculty_name: body.faculty_name,
    };
    await crudController.putModel({ table, id, values }, request, response);
};

const deleteFaculty = async (request, response) => {
    const id = {
        name: "faculty",
        value: request.params.facultyId.toString()
    };
    await crudController.deleteModel({ table, id }, request, response);
};

router.get("/", getFaculties);
router.post("/", postFaculty);
router.put("/", putFaculty)
router.delete("/:facultyId", deleteFaculty);

module.exports = router;