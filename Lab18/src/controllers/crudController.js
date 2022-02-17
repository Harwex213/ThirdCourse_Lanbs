const crudService = require("../services/crudService");

const getModels = async (info, request, response, next) => {
    try {
        response.status(200).json(await crudService(info.model).getAllModels());
    } catch (e) {
        next(e);
    }
};

const postModel = async (info, request, response, next) => {
    try {
        response.status(200).json(await crudService(info.model).createModel(info.id, info.values));
    } catch (e) {
        next(e);
    }
};

const putModel = async (info, request, response, next) => {
    try {
        response.status(200).json(await crudService(info.model).updateModel(info.id, info.values));
    } catch (e) {
        next(e);
    }
};

const deleteModel = async (info, request, response, next) => {
    try {
        response.status(200).json(await crudService(info.model).deleteModel(info.id));
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