let model = null;
const dataAccess = require("../data-access/mssql/data-access");

const getAllModels = async () => {
    return (await dataAccess(model).getEntities());
}

const getAllModelsWithScope = async (scope) => {
    return (await dataAccess(model).getEntitiesWithScope(scope));
}

const getAllModelsWithInclude = async (id, include) => {
    return (await dataAccess(model).getEntitiesWithInclude(id, include));
}

const createModel = async (id, values) => {
    return await dataAccess(model).createEntity(id, values);
}

const updateModel = async (id, values) => {
    return await dataAccess(model).updateEntity(id, values);
}

const deleteModel = async (id) => {
    return await dataAccess(model).deleteEntity(id);
}

module.exports = (manipulationModel) => {
    model = manipulationModel;
    return {
        getAllModels,
        getAllModelsWithScope,
        getAllModelsWithInclude,
        createModel,
        updateModel,
        deleteModel
    }
}