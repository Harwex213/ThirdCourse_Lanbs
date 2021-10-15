const tables = require("./dto/tablesDto");
const dataAccess = require("../data-access/data-access")(tables.auditoriumsService);

const getAllAuditoriums = async () => {
    return (await dataAccess.getEntities());
}

const createAuditorium = async (id, values) => {
    return await dataAccess.createEntity(id, values);
}

const updateAuditorium = async (id, values) => {
    return await dataAccess.updateEntity(id, values);
}

const deleteAuditorium = async (id) => {
    return await dataAccess.deleteEntity(id);
}

module.exports = {
    getAllAuditoriums,
    createAuditorium,
    updateAuditorium,
    deleteAuditorium
}