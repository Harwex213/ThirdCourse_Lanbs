const express = require("express");
const table = require("../services/tablesDto").auditoriumTypesService;
const crudController = require("./crudController");

const getAuditoriumTypes = (request, response, next) => {
    next({ table });
}

const postAuditoriumType = async (request, response, next) => {
    const body = request.body;
    const id = {
        name: "auditorium_type",
        value: body.auditorium_type
    };
    const values = [body.auditorium_type, body.auditorium_typename];
    next({ table, id, values });
};

const putAuditoriumType = async (request, response, next) => {
    const body = request.body;
    const id = {
        name: "auditorium_type",
        value: body.auditorium_type
    };
    const values = {
        auditorium_typename: body.auditorium_typename,
    };
    next({ table, id, values });
};

const deleteAuditoriumType = async (request, response, next) => {
    const id = {
        name: "auditorium_type",
        value: request.params.auditoriumTypeId.toString()
    };
    next({ table, id });
};

const router = express.Router();

router.get("/", getAuditoriumTypes, crudController.getModels);
router.post("/", postAuditoriumType, crudController.postModel);
router.put("/", putAuditoriumType, crudController.putModel)
router.delete("/:auditoriumTypeId", deleteAuditoriumType, crudController.deleteModel);

module.exports = router;