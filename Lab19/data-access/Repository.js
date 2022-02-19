const { sequelize } = require("./sequelize");
const Sequelize = require("sequelize");

Sequelize.DATE.prototype._stringify = function _stringify(date, options) {
    return this._applyTimezone(date, options).format('YYYY-MM-DD HH:mm:ss.SSS');
};

const getModels = (model) => {
    return sequelize.model(model).findAll({
        raw: true
    });
}

const postModel = async (model, values) => {
    return await sequelize.model(model).create(values);
}

const putModel = async (model, id, values) => {
    return await sequelize.model(model).update(values, {
        where: {
            ...id
        }
    });
}

const deleteModel = async (model, id) => {
    return await sequelize.model(model).destroy({ where: {
        ...id
    }});
}

module.exports = {
    getModels,
    postModel,
    putModel,
    deleteModel
}