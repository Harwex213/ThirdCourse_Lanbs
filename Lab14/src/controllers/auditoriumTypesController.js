const table = require("../dto/tables").auditoriumTypes;
const crudController = require("./crudController");
const router = require("../router/router")();

const getAuditoriumTypes = async (request, response) => {
    await crudController.getModels({ table }, request, response);
}

const postAuditoriumType = async (request, response) => {
    const body = request.body;
    const id = {
        name: "auditorium_type",
        value: body.auditorium_type
    };
    const values = {
        auditorium_type: body.auditorium_type,
        auditorium_typename: body.auditorium_typename,
    };
    await crudController.postModel({ table, id, values }, request, response);
};

const putAuditoriumType = async (request, response) => {
    const body = request.body;
    const id = {
        name: "auditorium_type",
        value: body.auditorium_type
    };
    const values = {
        auditorium_typename: body.auditorium_typename,
    };
    await crudController.putModel({ table, id, values }, request, response);
};

const deleteAuditoriumType = async (request, response) => {
    const id = {
        name: "auditorium_type",
        value: request.params.auditoriumTypeId.toString()
    };
    await crudController.deleteModel({ table, id }, request, response);
};

router.get("/", getAuditoriumTypes);
router.post("/", postAuditoriumType);
router.put("/", putAuditoriumType)
router.delete("/:auditoriumTypeId", deleteAuditoriumType);

module.exports = router;