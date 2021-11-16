const crudService = require("../services/crudService");

const getModels = async (info, request, response) => {
    const result = await crudService(info.table).getAllModels();

    response.writeHead(200, {
        "Content-Type" : "application/json"
    });
    response.end(JSON.stringify(result));
};

const postModel = async (info, request, response) => {
    const result = await crudService(info.table).createModel(info.id, info.values);

    response.writeHead(200, {
        "Content-Type" : "application/json"
    });
    response.end(JSON.stringify(result));
};

const putModel = async (info, request, response) => {
    const result = await crudService(info.table).updateModel(info.id, info.values);

    response.writeHead(200, {
        "Content-Type" : "application/json"
    });
    response.end(JSON.stringify(result));
};

const deleteModel = async (info, request, response) => {
    const result = await crudService(info.table).deleteModel(info.id);

    response.writeHead(200, {
        "Content-Type" : "application/json"
    });
    response.end(JSON.stringify(result));
};

module.exports = {
    getModels,
    postModel,
    putModel,
    deleteModel
}