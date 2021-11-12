const mongoose = require("mongoose");
const config = require("config");

const initializeConnection = async () => {
    try {
        await mongoose.connect(config.get("mongodb.uri"));
    } catch (e) {
        console.log("Connect to db error: ", e.message);
    }
};

initializeConnection();

const makeSelectString = (Entity) => {
    let selectString = "";
    for (const [key, _] of Object.entries(Entity.schema.obj)) {
        if (key === "_id") {
            continue;
        }
        selectString += `${key} `;
    }
    return selectString;
}

const getEntities = async (Entity) => {
    return await Entity.find({}, `-_id ${makeSelectString(Entity)}`);
};

const createEntity = async (Entity, id, values) => {
    if (await Entity.exists({ [id.name]: id.value })) {
        const error = new Error(`Entity with such ${id.name} = ${id.value} is existing already`);
        error.status = 400;
        throw error;
    }

    await Entity.create({
        _id: new mongoose.Types.ObjectId(),
        ...values
    });
    return values;
};

const updateEntity = async (Entity, id, values) => {
    if (!(await Entity.exists({ [id.name]: id.value }))) {
        const error = new Error("Not found");
        error.status = 404;
        throw error;
    }

    await Entity.findOneAndUpdate({ [id.name]: id.value }, values);
    return {
        [id.name]: id.value,
        ...values
    }
};

const deleteEntity = async (Entity, id) => {
    if (!(await Entity.exists({ [id.name]: id.value }))) {
        const error = new Error("Not found");
        error.status = 404;
        throw error;
    }

    return await Entity.findOneAndDelete({ [id.name]: id.value }).select(`-_id ${makeSelectString(Entity)}`);
};

module.exports = (Entity) => {
    return {
        getEntities: () => getEntities(Entity),
        createEntity: (id, values) => createEntity(Entity, id, values),
        updateEntity: (id, values) => updateEntity(Entity, id, values),
        deleteEntity: (id) => deleteEntity(Entity, id)
    }
}