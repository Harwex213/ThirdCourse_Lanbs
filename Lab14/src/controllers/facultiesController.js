const express = require("express");
const table = require("../services/tablesDto").facultiesService;
const crudController = require("./crudController");

const getFaculties = (request, response, next) => {
    next({ table });
}

const postFaculty = async (request, response, next) => {
    const body = request.body;
    const id = {
        name: "faculty",
        value: body.faculty
    };
    const values = [body.faculty, body.faculty_name];
    next({ table, id, values });
};

const putFaculty = async (request, response, next) => {
    const body = request.body;
    const id = {
        name: "faculty",
        value: body.faculty
    };
    const values = {
        faculty_name: body.faculty_name,
    };
    next({ table, id, values });
};

const deleteFaculty = async (request, response, next) => {
    const id = {
        name: "faculty",
        value: request.params.facultyId.toString()
    };
    next({ table, id });
};

const router = express.Router();

router.get("/", getFaculties, crudController.getModels);
router.post("/", postFaculty, crudController.postModel);
router.put("/", putFaculty, crudController.putModel)
router.delete("/:facultyId", deleteFaculty, crudController.deleteModel);

module.exports = router;