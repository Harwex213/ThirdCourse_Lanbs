const table = require("../dto/tables").auditoriums;
const crudController = require("./crudController");
const router = require("../router/router")();

const getAuditoriums = async (request, response) => {
    await crudController.getModels({ table }, request, response);
};

const postAuditorium = async (request, response) => {
    const body = request.body;
    const id = {
        name: "auditorium",
        value: body.auditorium
    };
    const values = {
        auditorium: body.auditorium,
        auditorium_name: body.auditorium_name,
        auditorium_capacity: body.auditorium_capacity,
        auditorium_type: body.auditorium_type
    };
    await crudController.postModel({ table, id, values }, request, response);
};

const putAuditorium = async (request, response) => {
    const body = request.body;
    const id = {
        name: "auditorium",
        value: body.auditorium
    };
    const values = {
        auditorium_name: body.auditorium_name,
        auditorium_capacity: body.auditorium_capacity,
        auditorium_type: body.auditorium_type
    };
    await crudController.putModel({ table, id, values }, request, response);
};

const deleteAuditorium = async (request, response) => {
    const id = {
        name: "auditorium",
        value: request.params.auditoriumId.toString()
    };
    await crudController.deleteModel({ table, id }, request, response);
};

router.get("/", getAuditoriums);
router.post("/", postAuditorium);
router.put("/", putAuditorium)
router.delete("/:auditoriumId", deleteAuditorium);

module.exports = router;