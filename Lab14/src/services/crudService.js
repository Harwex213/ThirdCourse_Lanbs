let table = null;
const config = require("config");
const dataAccess = require(`../data-access/${config.get("database")}/data-access`);

const getAllModels = async () => {
    return (await dataAccess(table).getEntities());
}

const createModel = async (id, values) => {
    return await dataAccess(table).createEntity(id, values);
}

const updateModel = async (id, values) => {
    return await dataAccess(table).updateEntity(id, values);
}

const deleteModel = async (id) => {
    return await dataAccess(table).deleteEntity(id);
}

module.exports = (manipulationTable) => {
    table = manipulationTable;
    return {
        getAllModels,
        createModel,
        updateModel,
        deleteModel
    }
}