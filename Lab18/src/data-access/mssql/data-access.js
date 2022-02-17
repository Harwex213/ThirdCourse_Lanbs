const { Sequelize } = require("sequelize");
const initModels = require("../models/initModels");

const sequelize = new Sequelize("University", "sa", "Passw0rd1", {
    host: "localhost",
    dialect: "mssql"
});

(async () => {
    try {
        await sequelize.authenticate();
        console.log("Connection has been established successfully.");

        initModels(sequelize);
        console.log("Defining models of database.");
    } catch (error) {
        console.error("Unable to connect to the database:", error);
    }
})();

const getEntities = (model) => sequelize.model(model).findAll({
    raw: true,
})

const getEntitiesWithScope = async (model, scope) => (await sequelize.model(model).scope(scope).findAll())
    .map(m => m.dataValues);

const getEntitiesWithInclude = async (model, id, include) => {
    const setModels = (include) => {
        include.forEach(obj => {
            obj.model = sequelize.model(obj.model);
            if (obj.include) {
                setModels(obj.include);
            }
        })
    }

    setModels(include);

    return (await sequelize.model(model).findAll({
        where: {
            ...id
        },
        include
    })).map(m => m.dataValues);
};

const createEntity = async (model, id, values) => sequelize.model(model).create({
    ...values
}, { raw: true })

const updateEntity = async (model, id, values) => sequelize.model(model).update({
    ...values
}, {
    where: {
        ...id
    }
})

const deleteEntity = async (model, id) => sequelize.model(model).destroy({
    where: {
        ...id
    }
})

module.exports = (model) => {
    return {
        getEntities: async () => await getEntities(model),
        getEntitiesWithScope: async (scope) => await getEntitiesWithScope(model, scope),
        getEntitiesWithInclude: async (id, include) => await getEntitiesWithInclude(model, id, include),
        createEntity: async (id, values) => await createEntity(model, id, values),
        updateEntity: async (id, values) => await updateEntity(model, id, values),
        deleteEntity: async (id) => await deleteEntity(model, id),
    }
}