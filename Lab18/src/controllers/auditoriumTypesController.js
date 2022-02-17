const express = require("express");
const model = require("../constants/database").AuditoriumType;
const crudController = require("./crudController");

const getAuditoriumTypes = (request, response, next) => {
    next({ model });
}

const postAuditoriumType = async (request, response, next) => {
    const body = request.body;
    const id = {
        AUDITORIUM_TYPE: body.auditorium_type
    };
    const values = {
        AUDITORIUM_TYPE: body.auditorium_type,
        AUDITORIUM_TYPENAME: body.auditorium_typename,
    };
    next({ model, id, values });
};

const putAuditoriumType = async (request, response, next) => {
    const body = request.body;
    const id = {
        AUDITORIUM_TYPE: body.auditorium_type
    };
    const values = {
        AUDITORIUM_TYPENAME: body.auditorium_typename,
    };
    next({ model, id, values });
};

const deleteAuditoriumType = async (request, response, next) => {
    const id = {
        AUDITORIUM_TYPE: request.params.auditoriumTypeId.toString()
    };
    next({ model, id });
};

const router = express.Router();

router.get("/", getAuditoriumTypes, crudController.getModels);
router.post("/", postAuditoriumType, crudController.postModel);
router.put("/", putAuditoriumType, crudController.putModel)
router.delete("/:auditoriumTypeId", deleteAuditoriumType, crudController.deleteModel);

module.exports = router;