const table = require("../dto/tables").pulpits;
const crudController = require("./crudController");
const router = require("../router/router")();

const getPulpits = async (request, response) => {
    await crudController.getModels({ table }, request, response);
}

const postPulpit = async (request, response) => {
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
    await crudController.postModel({ table, id, values }, request, response);
};

const putPulpit = async (request, response) => {
    const body = request.body;
    const id = {
        name: "pulpit",
        value: body.pulpit
    };
    const values = {
        pulpit_name: body.pulpit_name,
        faculty: body.faculty
    };
    await crudController.putModel({ table, id, values }, request, response);
};

const deletePulpit = async (request, response) => {
    const id = {
        name: "pulpit",
        value: request.params.pulpitId.toString()
    };
    await crudController.deleteModel({ table, id }, request, response);
};

router.get("/", getPulpits);
router.post("/", postPulpit);
router.put("/", putPulpit)
router.delete("/:pulpitId", deletePulpit);

module.exports = router;