const sql = require("mssql/msnodesqlv8");
const config = require("config");

const pool = new sql.ConnectionPool(config.get("mssql.config"));
const connectedPool = pool.connect();

const _getIdValue = (id) => typeof id.value === "string" ? `'${id.value}'` : id.value;

const tryFindEntity = async (table, idName, idValue) => {
    await connectedPool;

    const response = await pool.request().query(`Select * from ${table} where ${idName} = ${idValue}`);
    return {
        isExist: response.recordset.length !== 0,
        entity: response.recordset.length !== 0 ? response.recordset[0] : {}
    }
}

const getEntities = async (table) => {
    await connectedPool;

    return (await pool.request().query(`Select * from ${table}`)).recordset;
}

const createEntity = async (table, id, values) => {
    await connectedPool;

    const idValue = _getIdValue(id);
    const { isExist } = await tryFindEntity(table, id.name, idValue);
    if (isExist) {
        const error = new Error(`Entity with such ${id.name} = ${idValue} is existing already`);
        error.status = 400;
        throw error;
    }

    let valuesString = "";
    for (let i = 0; i < values.length; i++) {
        valuesString += typeof values[i] === "string" ? `'${values[i]}'` : values[i];
    }
    for (const [_, value] of Object.entries(values)) {
        valuesString += typeof value === "string" ? `'${value}'` : value;
        valuesString += `, `;
    }
    valuesString = valuesString.substring(0, valuesString.length - 2) + " ";

    await pool.request().query(`Insert into ${table} values(${valuesString})`);
    return (await pool.request().query(`Select * from ${table} where ${id.name} = ${idValue}`)).recordset[0];
}

const updateEntity = async (table, id, values) => {
    await connectedPool;

    const idValue = _getIdValue(id);
    const { isExist } = await tryFindEntity(table, id.name, idValue);
    if (!isExist) {
        const error = new Error("Not found");
        error.status = 404;
        throw error;
    }

    let updatedColumnsString = `set `;
    for (const [key, value] of Object.entries(values)) {
        if (key === id.name) {
            continue;
        }
        updatedColumnsString += `${key} = `;
        updatedColumnsString += typeof value === "string" ? `'${value}'` : `${value}`;
        updatedColumnsString += `, `;
    }
    updatedColumnsString = updatedColumnsString.substring(0, updatedColumnsString.length - 2) + " ";

    await pool.request().query(`Update ${table} ` + updatedColumnsString + `where ${id.name} = ${idValue}`);
    return (await pool.request().query(`Select * from ${table} where ${id.name} = ${idValue}`)).recordset[0];
}

const deleteEntity = async (table, id) => {
    await connectedPool;

    const idValue = _getIdValue(id);
    const { isExist, entity } = await tryFindEntity(table, id.name, idValue);
    if (!isExist) {
        const error = new Error("Not found");
        error.status = 404;
        throw error;
    }

    await pool.request().query(`Delete from ${table} where ${id.name} = ${idValue}`);
    return entity;
}

const query = async (queryString) => {
    await connectedPool;

    return (await pool.request().query(queryString)).recordset;
}

module.exports = (table) => {
    return {
        getEntities: async () => await getEntities(table),
        tryFindEntity: async (id) => await tryFindEntity(table, id.name, _getIdValue(id)),
        createEntity: async (id, values) => await createEntity(table, id, values),
        updateEntity: async (id, values) => await updateEntity(table, id, values),
        deleteEntity: async (id) => await deleteEntity(table, id),
        query
    }
}