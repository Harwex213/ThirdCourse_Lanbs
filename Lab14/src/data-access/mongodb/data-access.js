let Entity = null;
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

const makeSelectString = () => {
    let selectString = "";
    for (const [key, _] of Object.entries(Entity.schema.obj)) {
        if (key === "_id") {
            continue;
        }
        selectString += `${key} `;
    }
    return selectString;
}

const getEntities = async () => {
    return await Entity.find({}, `-_id ${makeSelectString()}`);
};

const createEntity = async (id, values) => {
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

const updateEntity = async (id, values) => {
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

const deleteEntity = async (id) => {
    if (!(await Entity.exists({ [id.name]: id.value }))) {
        const error = new Error("Not found");
        error.status = 404;
        throw error;
    }

    return await Entity.findOneAndDelete({ [id.name]: id.value }).select(`-_id ${makeSelectString()}`);
};

module.exports = (ManipulationEntity) => {
    Entity = ManipulationEntity;
    return {
        getEntities,
        createEntity,
        updateEntity,
        deleteEntity
    }
}