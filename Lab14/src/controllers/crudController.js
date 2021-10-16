const crudService = require("../services/crudService");

const getModels = async (info, request, response, next) => {
    try {
        response.status(200).json(await crudService(info.table).getAllModels());
    } catch (e) {
        next(e);
    }
};

const postModel = async (info, request, response, next) => {
    try {
        response.status(200).json(await crudService(info.table).createModel(info.id, info.values));
    } catch (e) {
        next(e);
    }
};

const putModel = async (info, request, response, next) => {
    try {
        response.status(200).json(await crudService(info.table).updateModel(info.id, info.values));
    } catch (e) {
        next(e);
    }
};

const deleteModel = async (info, request, response, next) => {
    try {
        response.status(200).json(await crudService(info.table).deleteModel(info.id));
    } catch (e) {
        next(e);
    }
};

module.exports = {
    getModels,
    postModel,
    putModel,
    deleteModel
}