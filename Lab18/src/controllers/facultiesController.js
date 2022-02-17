const express = require("express");
const { Faculty: model, Pulpit, Teacher } = require("../constants/database");
const crudController = require("./crudController");
const crudService = require("../services/crudService");

const getFaculties = (request, response, next) => {
    next({ model });
}

const getPulpitsOfFaculty = async (request, response, next) => {
    try {
        const id = {
            FACULTY: request.params.facultyId.toString()
        };

        response.status(200).json(await crudService(model).getAllModelsWithInclude(id,
            [
                {
                    model: Pulpit,
                    as: "PULPITs"
                }
        ]));
    } catch (e) {
        next(e);
    }
}

const getTeachersOfFaculty = async (request, response, next) => {
    try {
        const id = {
            FACULTY: request.params.facultyId.toString()
        };

        const data = await crudService(model).getAllModelsWithInclude(id, [
            {
                model: Pulpit,
                as: "PULPITs",
                include: [
                    {
                        model: Teacher,
                        as: "TEACHERs"
                    },
                ]
            },
        ]);

        const faculty = {
            FACULTY: data[0].FACULTY,
            FACULTY_NAME: data[0].FACULTY_NAME
        }

        const TEACHERs = data[0].PULPITs[0].TEACHERs.map(d => d.dataValues);

        response.status(200).json([{
            ...faculty,
            TEACHERs
        }]);
    } catch (e) {
        next(e);
    }
}

const postFaculty = async (request, response, next) => {
    const body = request.body;
    const id = {
        FACULTY: body.faculty
    };
    const values = {
        FACULTY: body.faculty,
        FACULTY_NAME: body.faculty_name,
    };
    next({ model, id, values });
};

const putFaculty = async (request, response, next) => {
    const body = request.body;
    const id = {
        FACULTY: body.faculty
    };
    const values = {
        FACULTY_NAME: body.FACULTY_NAME,
    };
    next({ model, id, values });
};

const deleteFaculty = async (request, response, next) => {
    const id = {
        FACULTY: request.params.facultyId.toString()
    };
    next({ model, id });
};

const router = express.Router();

router.get("/", getFaculties, crudController.getModels);
router.get("/:facultyId/pulpits", getPulpitsOfFaculty);
router.get("/:facultyId/teachers", getTeachersOfFaculty);
router.post("/", postFaculty, crudController.postModel);
router.put("/", putFaculty, crudController.putModel)
router.delete("/:facultyId", deleteFaculty, crudController.deleteModel);

module.exports = router;