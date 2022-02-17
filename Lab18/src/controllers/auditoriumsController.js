const express = require("express");
const model = require("../constants/database").Auditorium;
const crudController = require("./crudController")

const getAuditoriums = (request, response, next) => {
    next({ model });
};

const postAuditorium = async (request, response, next) => {
    const body = request.body;
    const id = {
        AUDITORIUM: body.auditorium
    };
    const values = {
        AUDITORIUM: body.auditorium,
        AUDITORIUM_NAME: body.auditorium_name,
        AUDITORIUM_CAPACITY: body.auditorium_capacity,
        AUDITORIUM_TYPE: body.auditorium_type
    };
    next({ model, id, values });
};

const putAuditorium = async (request, response, next) => {
    const body = request.body;
    const id = {
        AUDITORIUM: body.auditorium
    };
    const values = {
        AUDITORIUM_NAME: body.auditorium_name,
        AUDITORIUM_CAPACITY: body.auditorium_capacity,
        AUDITORIUM_TYPE: body.auditorium_type
    };
    next({ model, id, values });
};

const deleteAuditorium = async (request, response, next) => {
    const id = {
        AUDITORIUM: request.params.auditoriumId.toString()
    };
    next({ model, id });
};

const router = express.Router();

router.get("/", getAuditoriums, crudController.getModels);
router.post("/", postAuditorium, crudController.postModel);
router.put("/", putAuditorium, crudController.putModel)
router.delete("/:auditoriumId", deleteAuditorium, crudController.deleteModel);

module.exports = router;