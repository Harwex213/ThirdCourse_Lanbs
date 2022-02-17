const express = require("express");
const model = require("../constants/database").Pulpit;
const crudController = require("./crudController");

const getPulpits = (request, response, next) => {
    next({ model });
}

const postPulpit = async (request, response, next) => {
    const body = request.body;
    const id = {
        PULPIT: body.pulpit
    };
    const values = {
        PULPIT: body.pulpit,
        PULPIT_NAME: body.pulpit_name,
        FACULTY: body.faculty
    };
    next({ model, id, values });
};

const putPulpit = async (request, response, next) => {
    const body = request.body;
    const id = {
        PULPIT: body.pulpit
    };
    const values = {
        PULPIT_NAME: body.pulpit_name,
        FACULTY: body.faculty
    };
    next({ model, id, values });
};

const deletePulpit = async (request, response, next) => {
    const id = {
        PULPIT: request.params.pulpitId.toString()
    };
    next({ model, id });
};

const router = express.Router();

router.get("/", getPulpits, crudController.getModels);
router.post("/", postPulpit, crudController.postModel);
router.put("/", putPulpit, crudController.putModel)
router.delete("/:pulpitId", deletePulpit, crudController.deleteModel);

module.exports = router;