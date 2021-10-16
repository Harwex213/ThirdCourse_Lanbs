const express = require("express");
const table = require("../services/tablesDto").pulpitsService;
const crudController = require("./crudController");

const getPulpits = (request, response, next) => {
    next({ table });
}

const postPulpit = async (request, response, next) => {
    const body = request.body;
    const id = {
        name: "pulpit",
        value: body.pulpit
    };
    const values = {
        pulpit: body.pulpit,
        pulpit_name: body.pulpit_name,
        faculty: body.faculty
    };
    next({ table, id, values });
};

const putPulpit = async (request, response, next) => {
    const body = request.body;
    const id = {
        name: "pulpit",
        value: body.pulpit
    };
    const values = {
        pulpit_name: body.pulpit_name,
        faculty: body.faculty
    };
    next({ table, id, values });
};

const deletePulpit = async (request, response, next) => {
    const id = {
        name: "pulpit",
        value: request.params.pulpitId.toString()
    };
    next({ table, id });
};

const router = express.Router();

router.get("/", getPulpits, crudController.getModels);
router.post("/", postPulpit, crudController.postModel);
router.put("/", putPulpit, crudController.putModel)
router.delete("/:pulpitId", deletePulpit, crudController.deleteModel);

module.exports = router;